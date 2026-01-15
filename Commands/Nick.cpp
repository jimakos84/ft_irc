
#include "Nick.hpp"
#include "ParentCommand.hpp"
#include "../Client.hpp"
#include "../Server.hpp"

Nick::Nick() {}

Nick::~Nick() {}

bool Nick::cmdNeedsRegistration() const {
    return (false);
}

void Nick::executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) {
    if (cmdParams.size() == 0) {
        server->sendErrorMsg(client, ERR_NEEDMOREPARAMS, "More Parameters needed");
        return;
    }
    if (client.getNick() == "") {
        client.setNick(cmdParams[0]);
        client.setRegistered();
    }
    else
        changeNick(server, client, cmdParams[0]);
}

void Nick::changeNick(Server *server, Client &client_requesting_change, std::string new_nick) {
    std::map<int, Client> list = server->getClientList();
    for (auto it = list.begin(); it != list.end(); it++) {
        const Client &curr_client = it->second;
        std::string curr_client_nick = curr_client.getNick();
        if (curr_client_nick == new_nick) {
            server->sendErrorMsg(client_requesting_change, ERR_NICKNAMEINUSE, new_nick + " : Nickname is already in use");
            return ;
        }
    }
    client_requesting_change.setNick(new_nick);
}