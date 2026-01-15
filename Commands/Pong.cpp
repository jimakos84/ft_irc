
#include "Pong.hpp"
#include "ParentCommand.hpp"
#include "../Client.hpp"
#include "../Server.hpp"

Pong::Pong() {}

Pong::~Pong() {}
         
bool Pong::cmdNeedsRegistration() const {
    return (true);
}

void Pong::executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) {
    (void)server;
  
    if (cmdParams.size() == 0)
        return (server->sendErrorMsg(client, ERR_NEEDMOREPARAMS, "More Parameters needed"), void(0));    
    if (cmdParams[0] != server->getServerName())
        return (server->sendErrorMsg(client, ERR_NOSUCHSERVER, "No such server exists"), void(0));
    client.send(":" + server->getServerName() + " PONG :" + cmdParams[0] + "\r\n");
    return;
}