
#include "Pass.hpp"
#include "ParentCommand.hpp"
#include "../Client.hpp"
#include "../Server.hpp"

Pass::Pass() {}

Pass::~Pass() {}

bool Pass::cmdNeedsRegistration() const {
    return (false);
}

void Pass::executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) {
    if (cmdParams.size() == 0) {
        server->sendErrorMsg(client, ERR_NEEDMOREPARAMS, "More Parameters needed");
        server->removeClient(client.getFd());
        return;
    }
    if (cmdParams[0] != server->getPass()) {
        server->sendErrorMsg(client, ERR_PASSWDMISMATCH, "Password Incorrect");
        server->removeClient(client.getFd());
        return;
    }
    client.setPass();
    client.setRegistered();
}