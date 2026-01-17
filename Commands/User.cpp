
#include "User.hpp"
#include "ParentCommand.hpp"
#include "../Client.hpp"
#include "../Server.hpp"

User::User() {}

User::~User() {}

bool User::cmdNeedsRegistration() const {
    return (false);
}

void User::executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) {
    if (cmdParams.size() == 0 || cmdParams.size() < 4) {
        server->sendErrorMsg(client, ERR_NEEDMOREPARAMS, "More Parameters needed");
        return;
    }
    //probably needs some other checks but simple for now
    if (client.getUser() == "") {
        client.setUser(cmdParams[0]);
        client.setReal(cmdParams[3]);
        client.setRegistered();
    }
    //needs to be checked, can user be changed??
    client.setUser(cmdParams[0]);
    client.setReal(cmdParams[3]);
}