
#include "Client.hpp"

Client::Client(int fd, const std::string& hostname) : _fd(fd), _hostname(hostname) {}

void Client::appendtoClientBuffer(std::string incoming_data) {
	this->_buffer += incoming_data;
}

bool Client::completeMessageExists() {
	size_t found = _buffer.find("\r\n");
	if (found != std::string::npos)
		return (true);
	return (false);
}
		
std::string Client::extractMessage() {
	size_t found = _buffer.find("\r\n");
	std::string msg = _buffer.substr(0, found);
	_buffer.erase(0, found + 2);
	return (msg);
}

bool isRegistered() const {
	return (_isRegistered);
}

void Client::setRegistered() {
	this->_isRegistered = true;
	send();
}

void Client::setNick(const std::string &nick) {
	this->_nickname = nick;
}
	
void Client::setUser(const std::string &user, const std::string &real) {
	this->_username = user;
	this->_realname = real;
}

void Client::send(const std::string &msg) {
	size_t bytes_sent = std::send(this->_fd, msg.c_str(), msg.size(), 0);
	if (n == -1) {
		//handle error
	}
}