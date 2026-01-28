
#include "Nick.hpp"
#include "ParentCommand.hpp"
#include "../Client.hpp"
#include "../Server.hpp"
#include "../Channel.hpp"

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

void Nick::broadcastToChannels(Server *server, Client &client, std::string msg) {
    std::set<int> already_sent;
    const std::set<std::string>& channels = client.getJoinedChannels();
    for (std::set<std::string>::const_iterator it = channels.begin();
            it != channels.end();
            ++it)
    {
        Channel*    _channel = server->getChannelByName(server, *it);
        const std::set<Client*> &_members = _channel->getMembers();
        for (std::set<Client*>::const_iterator m = _members.begin(); m != _members.end(); ++m)
        {
            Client *other = *m;
            if (!other)
                continue;
            if (other->getFd() == client.getFd())
                continue;
            if (already_sent.insert(other->getFd()).second)
                other->sendMsg(msg);
        }
    }
}

void Nick::checkNickandSet(Server *server, Client &client, const std::string &new_nick)
{
    (void)server;
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
    std::string msg = ":" + client.getClientFullIdentifier() + " NICK " + ":" + new_nick + "\r\n";
    client.sendMsg(msg);
    broadcastToChannels(server, client, msg);
    client.setNick(new_nick);
}
