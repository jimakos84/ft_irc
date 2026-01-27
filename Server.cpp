
#include "Server.hpp"
#include <fcntl.h>

static volatile sig_atomic_t    g_running = 1;

static void handleSignal(int signum)
{
	(void)signum;
	g_running = 0;
}

static void setNonBlocking(int fd)
{
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("fcntl(F_SETFL) failed");
}

Server::Server(int port, const std::string& password)
	: _serverName("ft_irc.sadCats.fi"), _port(port), _password(password), _listenFd(-1) {}

Server::~Server()
{
	if (_listenFd != -1)
		close(_listenFd);
}

void    Server::start()
{
	std::signal(SIGINT, handleSignal);
	_listenFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_listenFd < 0)
		throw std::runtime_error("socket() failed");

	int yes = 1;
	setsockopt(_listenFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

	sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(_port);

	if (bind(_listenFd, (sockaddr*)&addr, sizeof(addr)) < 0)
		throw std::runtime_error("bind() failed");
	if (listen(_listenFd, SOMAXCONN) < 0)
		throw std::runtime_error("listen () failed");

	setNonBlocking(_listenFd);

	pollfd pfd;
	pfd.fd = _listenFd;
	pfd.events = POLLIN;
	pfd.revents = 0;

	_pollFds.push_back(pfd);

	std::cout   << "Server listening on port " << _port << std::endl;
}

void Server::run()
{
	while (g_running)
	{
		if (_pollFds.empty())
			continue;

		int ready = poll(&_pollFds[0], _pollFds.size(), 1000);
		if (ready < 0)
		{
			if (errno == EINTR)
				continue;
			throw std::runtime_error("poll() failed");
		}

		for (size_t i = _pollFds.size(); i-- > 0; )
		{
			short rev = _pollFds[i].revents;
			if (rev & (POLLHUP | POLLERR | POLLNVAL | POLLRDHUP))
			{
				if (_pollFds[i].fd != _listenFd)
					removeClient(_pollFds[i].fd);
				continue;
			}
			if (rev & POLLIN)
			{
				if (_pollFds[i].fd == _listenFd)
					acceptClient();
				else
				{
					receiveFromClient(_pollFds[i].fd);
				}
			}
		}
	}

	std::cout << "\nShutting down server..." << std::endl;
	for (auto& pair : _clients)
		close(pair.first);
	if (_listenFd != -1)
		close(_listenFd);
}


void    Server::acceptClient()
{
	sockaddr_in addr;
	socklen_t len = sizeof(addr);

	int clientFd = accept(_listenFd, (sockaddr*)&addr, &len);
	if (clientFd < 0)
		return;

	setNonBlocking(clientFd);

	pollfd pfd;
	pfd.fd = clientFd;
	pfd.events = POLLIN;
	pfd.revents = 0;

	_pollFds.push_back(pfd);
	char ip_str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &addr.sin_addr, ip_str, sizeof(ip_str));
    std::string client_ip(ip_str);
	_clients.insert(std::make_pair(clientFd, Client(clientFd, client_ip)));

	std::cout << "Client connected: fd=" << clientFd << std::endl;
}

void    Server::receiveFromClient(int fd)
{
	std::string buffer;
	buffer.resize(512);
	ssize_t bytes = recv(fd, &buffer[0], buffer.size(), 0);

	if (bytes < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return;
		removeClient(fd);
		return;
	}
	if (bytes == 0)
	{
		removeClient(fd);
		return;
	}
	buffer.resize(bytes);
	std::cout << "Buffer: " << buffer << std::endl;

	Client& client = _clients.at(fd);
	client.appendtoClientBuffer(buffer);

	while (client.completeMessageExists() == true)
	{
		std::string msg = client.extractMessage();
		if (!commandExecute(client, msg))
			break;
	}
}

bool Server::commandExecute(Client &client, std::string full_cmd)
{
    std::string cmdName;
    std::vector<std::string> cmdParams;
    Parser parser;
    parser.splitIrcLine(full_cmd, cmdName, cmdParams);

    if (cmdName.empty()) {
        return true; }

	std::transform(cmdName.begin(), cmdName.end(), cmdName.begin(), toupper);

	if (cmdName == "CAP" || cmdName == "WHO" || cmdName == "PONG" || cmdName == "WHOIS")
        return true;
    ParentCommand* cmd = _commandList.getCmd(cmdName);
	if (!cmd)
		return (sendErrorMsg(client, ERR_UNKNOWNCOMMAND, "Unknown command : " + cmdName), true);
	if (cmd->cmdNeedsRegistration() == true && client.isRegistered() == false)
		return (sendErrorMsg(client, ERR_NOTREGISTERED, "You have not registered"), removeClient(client.getFd()), false);
	if (cmd->isPassSet(client) == false && cmdName != "PASS")
		return (sendErrorMsg(client, ERR_NOTREGISTERED, "Password required first for registration"), removeClient(client.getFd()), false);
	cmd->executeCmd(this, client, cmdParams);
    return true;
}


void    Server::removeClient(int fd)
{
	close(fd);
	_clients.erase(fd);

	for (size_t i = 0; i < _pollFds.size(); ++i)
	{
		if (_pollFds[i].fd == fd)
		{
			_pollFds.erase(_pollFds.begin() + i);
			break;
		}
	}
	std::cout << "Client disconnected: fd=" << fd << std::endl;
}

void Server::sendReplyMsg(Client &client, std::string code, const std::string &msg)
{
    std::string reply = ":" + _serverName + " " + code + " " + client.getNick()
                      + " " + msg + "\r\n";
    client.sendMsg(reply);
}


void Server::sendErrorMsg(Client &client, std::string err_code, const std::string err_msg) {
	std::string error_msg = ":" + _serverName + " " + err_code + " " + client.getNick() + " " + err_msg + "\r\n";
	client.sendMsg(error_msg);
}

std::string Server::getServerName() const {
	return (_serverName);
}

std::string Server::getPass() const {
	return (_password);
}

std::map<int, Client>& Server::getClientList() { return _clients; }
const std::map<int, Client>& Server::getClientList() const { return _clients; }

std::map<std::string, Channel>& Server::getChannelList() { return _channels; }
const std::map<std::string, Channel>& Server::getChannelList() const { return _channels; }


void Server::addNewChannel(std::string channel_Name, Client &client) {
	auto result = _channels.emplace(channel_Name, Channel(channel_Name));	//returns std::pair<iterator, bool> wher iterator to the element (created or existing), bool/second if insertion happened or not
	if (result.second == true) {
		Channel &channel = result.first->second;
		channel.addClientToOperatorList(&client);
	}
}

void Server::removeChannel(std::string channel_name) {
	//delete
	for (auto& [existing_channel, _] : _channels) {
		std::cout << "Existing chan before: " << existing_channel << std::endl;
	}

	_channels.erase(channel_name);

	//delete
	for (auto& [chans, _] : _channels) {
		std::cout << "After: " << chans << std::endl;
	}
}

void Server::sendErrNicknameInUse(Client &client, const std::string &attemptedNick)
{
    std::string curr = client.getNick().empty() ? "*" : client.getNick();

    std::string msg = ":" + _serverName
        + " 433 " + curr + " " + attemptedNick
        + " :Nickname is already in use\r\n";

    client.sendMsg(msg);
}

void    Server::removeChannel(std::string channel_name) {
	std::map<std::string, Channel>& channel_list = getChannelList();
	
	std::map<std::string, Channel>::iterator it = channel_list.find(channel_name);
	if (it == channel_list.end())
		return ;
	channel_list.erase(it);
}
