
#include "Server.hpp"
#include "Parser.hpp"
#include <fcntl.h>

//command includes will be changed later
// #include "./Commands/ParentCommand.hpp"
// #include "CmdCenter.hpp"

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
					// handleClientComm(_pollFds[i].fd);
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
	_clients.insert(std::make_pair(clientFd, Client(clientFd)));

	std::cout << "Client connected: fd=" << clientFd << std::endl;
}

// void    Server::handleClientComm(int client_fd) {
// 	Client& client = _clients.at(client_fd);
// 	receiveFromClient(client, client_fd);
// 	if (client.isRegistered() == false) {
// 		std::cout << "not registered" << std::endl;
// 		// removeClient(client_fd);
// 	}
// }

// void    Server::receiveFromClient(Client &client, int fd)
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
		// std::cout << "\n\n[PARSED] " << msg << std::endl;
		if (!commandExecute(client, msg))
			break;
	}
	//if (client.isRegistered() == false)
	//	std::cout << "not registered" << std::endl;
}



bool    Server::isRegistrationCmd(const std::string &cmdName)
{
	std::string possibleCmd[] = {"PASS", "NICK", "USER"};
	for (unsigned long i = 0; i < 3; i++) {
		if (cmdName == possibleCmd[i])
			return (true);
	}
	return (false);
}

bool Server::startRegistration(Client &client,
                               const std::string &cmdName,
                               const std::vector<std::string> &cmdParams)
{
    if (cmdName == "PASS")
    {
        if (cmdParams.size() >= 1 && cmdParams[0] == _password)
            client.setPass();
        else
        {
            client.send("ERROR :Password incorrect\r\n");
            removeClient(client.getFd());
            return false;
        }
    }
    else if (cmdName == "NICK")
    {
        if (cmdParams.size() >= 1 && !cmdParams[0].empty())
            client.setNick(cmdParams[0]);
        else
            client.send("ERROR :Need more parameters\r\n");
    }
    else if (cmdName == "USER")
    {
        if (cmdParams.size() >= 1 && !cmdParams[0].empty())
        {
            client.setUser(cmdParams[0]);
            if (cmdParams.size() >= 4 && !cmdParams[3].empty())
                client.setReal(cmdParams[3]);
        }
        else
            client.send("ERROR :Need more parameters\r\n");
    }
    return true;
}

void Server::sendWelcomeMsg(Client &client) {
	std::string client_nick = client.getNick();
	// std::string server_name = _serverName;
	client.send(":" + std::string("ft_irc.sadCats.fi ") + RPL_WELCOME + " " + client_nick + " :Welcome to the Internet Relay Network " + client_nick + "\r\n");
	client.send(":" + std::string("ft_irc.sadCats.fi ") + RPL_YOURHOST + " " + client_nick + " :Your host is ft_irc.sadCats.fi\r\n");
	client.send(":" + std::string("ft_irc.sadCats.fi ") + RPL_CREATED + " " + client_nick + " :This server was created today\r\n");

}

bool Server::commandExecute(Client &client, std::string full_cmd)
{
    std::string cmdName;
    std::vector<std::string> cmdParams;
    Parser parser;
    parser.splitIrcLine(full_cmd, cmdName, cmdParams);

    if (cmdName.empty())
        return true;

    if (cmdName == "CAP" || cmdName == "WHO")
        return true;

    if (!client.isRegistered())
    {
        if (!isRegistrationCmd(cmdName))
        {
            client.send("ERROR :You have not registered\r\n"); // later: numeric 451
            return true; // keep connection open
        }

        if (!startRegistration(client, cmdName, cmdParams))
            return false; // startRegistration removed the client

        client.setRegistered();

        if (client.isRegistered())
            sendWelcomeMsg(client);

        return true; // IMPORTANT: do not fall through to "registered" logic
    }

    // Only registered users reach here
    std::cout << "I am registered, other commands" << std::endl;
	ParentCommand* cmd = _commandList.getCmd(cmdName);
	if (!cmd)
		return (client.send("Unknown cmd"), false) ;
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

// void    Server::sendToClient(int fd, const std::string& message) {
//     if (send(fd, (const void*)message.c_str(), message.length(), MSG_NOSIGNAL) == -1)
//         std::
// }

std::string Server::getServerName() const {
	return (_serverName);
}