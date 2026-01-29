
#include "Part.hpp"
#include "ParentCommand.hpp"
#include "../Client.hpp"
#include "../Server.hpp"

Part::Part() {}

Part::~Part() {}

bool Part::cmdNeedsRegistration() const {
    return (true);
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

    for (std::string channel : channel_args) {
        if (channel.empty() || channel[0] != '#')
            continue;
        std::string msg = ":" + client.getClientFullIdentifier() + " PART " + channel + "\r\n";
        if (cmdParams.size() > 1 && !cmdParams[1].empty())
            msg = ":" + client.getClientFullIdentifier() + " PART " + channel + " " + cmdParams[1] + " \r\n";
        Channel *chan = server->getChannelByName(server, channel);
        if (!chan) {
            server->sendErrorMsg(client, ERR_NOSUCHCHANNEL, channel + " :No such channel");
            continue;
        }
        const std::set<std::string> &joined_chans = client.getJoinedChannels();
        if (joined_chans.find(channel) == joined_chans.end())
        {
            server->sendErrorMsg(client, ERR_NOTONCHANNEL,
                                channel + " :Cannot send to channel");
            return;
        }
        client.sendMsg(msg);
        broadcastToChannel(client, *chan, msg);
        chan->removeClientFromMemberList(&client);
        if (chan->isClientOperator(&client))
            chan->removeClientFromOperatorList(&client);
        client.leaveChannel(channel);
        if (isChannelEmpty(*chan))
            server->removeChannel(channel);
    }
}
