
#include "Server.hpp"
#include "Parser.hpp"
#include <fcntl.h>

//command includes will be changed later
// #include "./Commands/ParentCommand.hpp"
// #include "./Commands/TestCmd.hpp"

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
	: _port(port), _password(password), _listenFd(-1) {}

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
		commandExecute(client, msg);
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

void	Server::startRegistration(Client &client, std::string cmdName, std::vector<std::string> cmdParams) {
	if (cmdName == "PASS") {
		if (cmdParams.size() > 0 && cmdParams[0] == _password)
			client.setPass();
		else {
			client.send("ERROR :Password incorrect\r\n");
			removeClient(client.getFd());
       		return;
		}
	}
	
	else if (cmdName == "NICK") {
		if (!cmdParams[0].empty())
			client.setNick(cmdParams[0]);
	}
	else if (cmdName == "USER") {
		if (cmdParams.size() > 0 && !cmdParams[0].empty()) {
			client.setUser(cmdParams[0]);
			if (cmdParams.size() > 3 && !cmdParams[3].empty())
				client.setReal(cmdParams[3]);
		}
	}
}

void Server::sendWelcomeMsg(Client &client) {
	std::string client_nick = client.getNick();
	client.send(":" + std::string("ft_irc ") + RPL_WELCOME + " " + client_nick + " :Welcome to the Internet Relay Network " + client_nick + "\r\n");
	client.send(":" + std::string("ft_irc ") + RPL_YOURHOST + " " + client_nick + " :Your host is ft_irc\r\n");
	client.send(":" + std::string("ft_irc ") + RPL_CREATED + " " + client_nick + " :This server was created today\r\n");

}

void    Server::commandExecute(Client &client, std::string full_cmd)
{
	std::string cmdName;
	std::vector<std::string> cmdParams;
	Parser parser;
	parser.splitIrcLine(full_cmd, cmdName, cmdParams);

	if (cmdName == "CAP" || cmdName == "WHO")
			return ;
	if (client.isRegistered() == false) {
		if (isRegistrationCmd(cmdName) == false) 
			std::cout << "Not registered missing params" << std::endl;
		startRegistration(client, cmdName, cmdParams);
		client.setRegistered();
		if (client.isRegistered()) {
			std::cout << "successful register" << std::endl;
			sendWelcomeMsg(client);
		}
	}

	//DELETE
	std::cout << "Nick: " << client.getNick() << std::endl;
	std::cout << "User: " << client.getUser() << std::endl;
	std::cout << "Real: " << client.getReal() << std::endl;
	//until here

	// if (client.isRegistered() == false)
	// 	removeClient(client.getFd());
	
	if (client.isRegistered())  {	//this can be changed to else later
	 	std::cout << "I am registered, other commands" << std::endl;
		// TestCmd test_cmd;
		// test_cmd.cmdEx();
	}
	
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