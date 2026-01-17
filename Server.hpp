
#pragma once

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
#include "MacrosDefinitions.hpp"
#include "CmdCenter.hpp"


class Server
{
private:
    void    acceptClient();
    void    receiveFromClient(int fd);
    // void    removeClient(int fd);
    // void    sendToClient(int fd, const std::string& message);
    // void    handleClientComm(int client_fd);
    // void    receiveFromClient(Client &client, int client_fd);
    void sendWelcomeMsg(Client &client);

    std::string             _serverName;
    int                     _port;
    std::string             _password;
    int                     _listenFd;

    std::vector<pollfd>     _pollFds;
    std::map<int, Client>   _clients;
    CmdCenter               _commandList;

public:
    Server(int  port, const std::string& password);
    ~Server();

    void    start();
    void    run();
    bool    commandExecute(Client &client, std::string cmd);
    bool    isRegistrationCmd(const std::string &cmdName);
    bool    startRegistration(Client &client,
                       const std::string &cmdName,
                       const std::vector<std::string> &cmdParams);
     void    removeClient(int fd);

    //get functions
    std::string getServerName() const;
    std::string getPass() const;
    std::map<int, Client> getClientList() const;

    //error
    void sendErrorMsg(Client &client, std::string err_code, const std::string err_msg);
};
