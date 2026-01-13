
#pragma once

#include <string>

class Client {
public:
    Client(int fd);

    int getFd() const;
    std::string& getBuffer();

private:
    int _fd;
    std::string _buffer;
};