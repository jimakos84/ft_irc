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

static bool isOnlyWhitespace(const std::string& s)
{
    for (char c : s)
        if (!std::isspace(static_cast<unsigned char>(c)))
            return false;
    return true;
}

void Topic::executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) {
    if (cmdParams.size() == 0) {
        server->sendErrorMsg(client, ERR_NEEDMOREPARAMS, "More Parameters needed for Topic");
        return;
    }
    std::string channelName = cmdParams[0];

    Channel * channel = getChannelByName(server, channelName);

    if (!channel)
    {
        server->sendErrorMsg(client, ERR_NOSUCHCHANNEL, channelName + " :No such channel");
        return ;
    }
    if (cmdParams.size() < 2)
    {
        if (channel->getTopic() == "")
        {    
            server->sendReplyMsg(client, RPL_NOTOPIC, channel->getChannelName() + " :No topic is set");
        }
        else
            server->sendReplyMsg(client, RPL_TOPIC, channel->getChannelName() + " :" + channel->getTopic());
        return ;
    }
    if (channel->getTopicRestrictionStatus() == true)
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
        return ;
    }
    std::string newTopic = cmdParams[1];

    if (newTopic.empty() || isOnlyWhitespace(newTopic))
    {
        channel->setTopic("");
        server->sendReplyMsg(client, RPL_NOTOPIC, channel->getChannelName() + " :Topic is removed");
        return ;
    }
    
    channel->setTopic(newTopic);
    std::string msg = channel->getChannelName() + " :" + channel->getTopic() + "\r\n";
    client.sendMsg(channel->getChannelName() + " :" + newTopic + "\r\n");
    return ;
}