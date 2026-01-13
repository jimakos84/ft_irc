
#include "Client.hpp"

Client::Client(int fd) : _fd(fd), _hostname(""), _nickname(""), _realname(""),
				 _username(""), _buffer(""), _isRegistered(false), _hasPass(false),
				_hasNick(false), _hasUser(false) {} //add is operator

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
	return (msg);
}

bool Client::isRegistered() const {
	return (_isRegistered);
}

void Client::setRegistered() {
	if (_hasPass && _hasNick && _hasUser) {
		this->_isRegistered = true;
		// send();
		std::cout << "registered" << std::endl;
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


void Client::send(const std::string &msg) {
	// size_t bytes_sent = std::send(this->_fd, msg.c_str(), msg.size(), 0);
	// if (bytes_sent == -1) {
	// 	print_err("Send failed", "", void(0));
	// }

	std::cout << "Client: " << _fd << " :: " << msg << std::endl;
}


