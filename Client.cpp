
#include "Client.hpp"

Client::Client(int fd) : _fd(fd) {}

int Client::getFd() const
{
    return _fd;
}

std::string& Client::getBuffer()
{
    return _buffer;
}
