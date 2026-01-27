
#include "Part.hpp"
#include "ParentCommand.hpp"
#include "../Client.hpp"
#include "../Server.hpp"

Part::Part() {}

Part::~Part() {}

bool Part::cmdNeedsRegistration() const {
    return (true);
}

Channel* Part::getChannelByName(Server *server, const std::string& name)
{
    std::map<std::string, Channel>& channel_list = server->getChannelList();
    auto it = channel_list.find(name);
    if (it != channel_list.end())
    {
        return (&it->second);
    }
    return (nullptr);
}

bool isChannelEmpty(Channel &chan) {
    std::set<Client*> members = chan.getMembers();
    if (members.empty())
        return true;
    else
        return false;
}

void broadcastToChannel(Client &client, Channel &chan, std::string msg) {
    std::set<int> already_sent;
    const std::set<Client*> &_members = chan.getMembers();
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

void Part::executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) {
    if (cmdParams.size() == 0) {
        server->sendErrorMsg(client, ERR_NEEDMOREPARAMS, "More Parameters needed for Part");
        return;
    }
    std::set<int> already_sent;
    std::vector<std::string> channel_args = splitLine(cmdParams[0], ',');

    for (std::string chans : channel_args) {
        if (chans.empty() || chans[0] != '#')
            continue;
        std::string msg = ":" + client.getClientFullIdentifier() + " PART " + chans + "\r\n";
        Channel *chan = getChannelByName(server, chans);
        if (!chan) {
            server->sendErrorMsg(client, ERR_NOSUCHCHANNEL, chans + " :No such channel");
            continue;
        }
        client.sendMsg(msg);
        broadcastToChannel(client, *chan, msg);
        chan->removeClientFromMemberList(&client);
        client.leaveChannel(chans);
        if (isChannelEmpty(*chan))
            server->removeChannel(chans);
    }
}
