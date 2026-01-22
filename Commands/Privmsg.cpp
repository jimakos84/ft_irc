
#include "Privmsg.hpp"
#include "../Client.hpp"
#include "../Server.hpp"
#include "../Channel.hpp"

Privmsg::Privmsg() {}

Privmsg::~Privmsg() {}

bool Privmsg::cmdNeedsRegistration() const {
    return (true);
}

void Privmsg::ChannelReplyMsg(Server *server, Client &client, Channel &channel) const
{
    (void)server;
    std::string Privmsg_msg = ":" + client.getClientFullIdentifier()
                         + " PRIVMSG " + channel.getChannelName() + "\r\n";
    const std::set<Client*>& members = channel.getMembers();
    for (Client* member : members){
        if (member != &client)
            member->sendMsg(Privmsg_msg);
    }
}

void    Privmsg::executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) {
    if (cmdParams.size() == 0) {
        server->sendErrorMsg(client, ERR_NEEDMOREPARAMS, "More Parameters needed for Join");
        return;
    }
    std::map<std::string, Channel>& channel_list = server->getChannelList();
        auto it = channel_list.find(cmdParams[0]);
        if (it == channel_list.end())
            return;
        Channel& channel = it->second;
    ChannelReplyMsg(server, client, channel);
}