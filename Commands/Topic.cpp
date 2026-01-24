#include "Topic.hpp"
#include "ParentCommand.hpp"
#include "../Client.hpp"
#include "../Server.hpp"
#include "../Channel.hpp"


Topic::Topic() {}

Topic::~Topic() {}

bool Topic::cmdNeedsRegistration() const {
    return (true);
}

Channel* Topic::getChannelByName(Server *server, const std::string& name)
{
    std::map<std::string, Channel>& channel_list = server->getChannelList();
    auto it = channel_list.find(name);
    if (it != channel_list.end())
    {
        return (&it->second);
    }
    return (nullptr);
}

void Topic::executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) {
    if (cmdParams.size() == 0) {
        server->sendErrorMsg(client, ERR_NEEDMOREPARAMS, "More Parameters needed for Join");
        return;
    }
    std::string channelName = cmdParams[0];

    Channel * channel = getChannelByName(server, channelName);

    if (!channel)
    {
        server->sendErrorMsg(client, ERR_NOTONCHANNEL, channelName + " :No such channel");
        return ;
    }
    if (cmdParams.size() == 1)
    {
        if (channel->getTopic() == "")
            server->sendReplyMsg(client, RPL_NOTOPIC, channel->getChannelName() + ":No topic is set");
        else
            server->sendReplyMsg(client, RPL_TOPIC, channel->getChannelName() + " 's Topic:" + channel->getTopic());
    }
    if (channel->getInviteOnlyStatus() == true)
    {
        if (channel->isClientOperator(&client) == false)
        {
            server->sendErrorMsg(client, ERR_CHANOPRIVSNEEDED, client.getNick() + " :You are not operator");
            return ;
        }
    }
    if (channel->isClientMember(&client) == false)
    {
        server->sendErrorMsg(client, ERR_NOTONCHANNEL, client.getNick() + " :You are not on channel ");
    }
    if (cmdParams[1].empty())
    {
        channel->setTopic("");
        server->sendReplyMsg(client, RPL_NOTOPIC, channel->getChannelName() + " :Topic is removed");
    }
    std::string newTopic = cmdParams[1];
    channel->setTopic(newTopic);
    server->sendReplyMsg(client, RPL_TOPIC, channel->getChannelName() + " :New topic is set " + newTopic);
}