
#include "Ping.hpp"
#include "ParentCommand.hpp"
#include "../Client.hpp"
#include "../Server.hpp"

Ping::Ping() {}

Ping::~Ping() {}

bool Ping::cmdNeedsRegistration() const {
    return (true);
}

void Ping::executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) {
    if (cmdParams.size() == 0)
        return (server->sendErrorMsg(client, ERR_NEEDMOREPARAMS, "More Parameters needed for Ping"), void(0));
    if (cmdParams[0] != server->getServerName())
        return (server->sendErrorMsg(client, ERR_NOSUCHSERVER, "No such server exists"), void(0));
    client.sendMsg(":" + server->getServerName() + " Ping :" + cmdParams[0] + "\r\n");
    return;
}
