
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
        server->sendErrorMsg(client, ERR_NEEDMOREPARAMS, "More Parameters needed for Nick");
        return;
    }
    if (client.getNick() == "") {
        checkNickandSet(server, client, cmdParams[0]);
        client.setRegistered();
    }
    else
        checkNickandSet(server, client, cmdParams[0]);
}

void Nick::checkNickandSet(Server *server, Client &client, const std::string &new_nick)
{
    if (new_nick == client.getNick())
        return;

    std::map<int, Client> &list = server->getClientList();
    for (std::map<int, Client>::iterator it = list.begin(); it != list.end(); ++it)
    {
        if (it->first == client.getFd())
            continue;
        if (it->second.getNick() == new_nick)
        {
            server->sendErrNicknameInUse(client, new_nick);
            return;
        }
    }
    client.setNick(new_nick);
}
