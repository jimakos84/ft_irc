
#include "Client.hpp"

Client::Client() {}
Client::Client(int fd, std::string host) : _fd(fd), _hostname(host), _nickname(""), _realname(""),
				 _username(""), _buffer(""), _isRegistered(false), _isOperator(false), _hasPass(false),
				_hasNick(false), _hasUser(false) {}//add is operator

void Client::appendtoClientBuffer(std::string incoming_data) {
	this->_buffer += incoming_data;
}

bool Client::completeMessageExists() {
	size_t found = _buffer.find("\n");
	if (found != std::string::npos)
		return (true);
	return (false);
}

std::string Client::extractMessage() {
	size_t found = _buffer.find("\n");
	std::string msg = _buffer.substr(0, found);
	_buffer.erase(0, found + 1);
	if (!msg.empty() && msg[msg.size() - 1] == '\r')
		msg.erase(msg.size() - 1);
	return (msg);
}

bool Client::isRegistered() const {
	return (_isRegistered);
}

bool Client::isOperator() const{
	return (_isOperator);
}

void Client::setRegistered() {
	if (_hasPass && _hasNick && _hasUser) {
		this->_isRegistered = true;
		sendWelcomeMsg();
		std::cout << "registered" << std::endl; //delete
	}
}

bool Client::hasPass() {
	return (_hasPass);
}

void Client::setPass() {
	this->_hasPass = true;
}

void Client::setNick(const std::string &nick) {
	this->_nickname = nick;
	_hasNick = true;
}

void Client::setUser(const std::string &user) {
	this->_username = user;
	_hasUser = true;
}

void Client::setReal(const std::string &real) {
	this->_realname = real;
}

int Client::getFd() const {
	return (this->_fd);
}

std::string Client::getNick() const {
	return (this->_nickname);
}

std::string Client::getUser() const {
	return (this->_username);
}

std::string Client::getReal() const {
	return (this->_realname);
}

std::string	Client::getClientFullIdentifier() const {
	return (_nickname + "!" + _username + "@" + _hostname);
}


void Client::sendMsg(const std::string &msg) {
	ssize_t bytes_sent = ::send(this->_fd, msg.c_str(), msg.size(), 0);
	 if (bytes_sent == -1) {
	 	print_err("Send failed", ""), void(0);
	 }
}

void Client::sendWelcomeMsg() {
	std::string client_nick = _nickname;
	sendMsg(":" + std::string("ft_irc.sadCats.fi ") + RPL_WELCOME + " " + client_nick + " :Welcome to the Internet Relay Network " + client_nick + "\r\n");
	sendMsg(":" + std::string("ft_irc.sadCats.fi ") + RPL_YOURHOST + " " + client_nick + " :Your host is ft_irc.sadCats.fi\r\n");
	sendMsg(":" + std::string("ft_irc.sadCats.fi ") + RPL_CREATED + " " + client_nick + " :This server was created today\r\n");
}

void Client::addJoinedChannel(std::string channel_joined) {
	_joined_channels.insert(channel_joined);
}

void Client::leaveChannel(std::string channel_to_leave) {
	_joined_channels.erase(channel_to_leave);
}
