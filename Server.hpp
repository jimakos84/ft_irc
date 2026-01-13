
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <map>
#include <poll.h>
#include <csignal>
#include "Client.hpp"
#include "utils.hpp"

class Server
{
private:
    void    acceptClient();
    void    receiveFromClient(int fd);
    void    removeClient(int fd);
    void    sendToClient(int fd, const std::string& message);

    int                     _port;
    std::string             _password;
    int                     _listenFd;

    std::vector<pollfd>     _pollFds;
    std::map<int, Client>   _clients;
public:
    Server(int  port, const std::string& password);
    ~Server();

    void    start();
    void    run();
};
