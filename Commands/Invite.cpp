#include "Invite.hpp"
#include "ParentCommand.hpp"
#include "../Client.hpp"
#include "../Server.hpp"
#include "../Channel.hpp"


Invite::Invite() {}

Invite::~Invite() {}

bool Invite::cmdNeedsRegistration() const {
    return (true);
}

void Invite::sendInvitationMsg(Server* server, Client& client, Client& invitee, Channel* channel)
{
    server->sendReplyMsg(client, RPL_INVITING, "Invitation sent to " + invitee.getNick() + " to join "  + channel->getChannelName());
    server->sendReplyMsg(invitee, RPL_INVITING, client.getNick() + " INVITE "  + invitee.getNick() + " to " + channel->getChannelName());
    return ;
}

Client* Invite::getClientByNick(Server* server, const std::string& nickName)
{
    auto& client_list = server->getClientList();

    for (auto it = client_list.begin(); it != client_list.end(); ++it)
    {
        if (it->second.getNick() == nickName)
        {
            return &it->second;
        }
    }
    return nullptr;
}

Channel* Invite::getChannelByName(Server *server, const std::string& name)
{
    std::map<std::string, Channel>& channel_list = server->getChannelList();
    auto it = channel_list.find(name);
    if (it != channel_list.end())
    {
        return (&it->second);
    }
    return (nullptr);
}

void Invite::executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) {
    if (cmdParams.size() <= 1) {
        server->sendErrorMsg(client, ERR_NEEDMOREPARAMS, "More Parameters needed for Join");
        return;
    }

    std::string inviteeName = cmdParams[0];
    std::string channelName = cmdParams[1];  
    
    Client* invitee = getClientByNick(server, inviteeName);
    if (!invitee)
    {
        server->sendErrorMsg(client, ERR_NOSUCHNICK, inviteeName + " :No such nick");
        return ;
    }

    Channel* channel = getChannelByName(server, channelName);
    if (!channel)
    {
        server->sendErrorMsg(client, ERR_NOTONCHANNEL, channelName + " :No such channel");
        return ;
    }
    if (channel->getInviteOnlyStatus())
    {
        if (channel->isClientOperator(&client) == false)
        {
            server->sendErrorMsg(client, ERR_CHANOPRIVSNEEDED, channelName + " :You're not channel operator");
            return ;
        }
        
    }
    if (channel->addInvitedClient(invitee, inviteeName) == ALREADY_MEMBER) // check with other clients
    {
        server->sendErrorMsg(client, ERR_USERONCHANNEL, inviteeName + " is already on channel");
        return ;
    }

    sendInvitationMsg(server, client, *invitee, channel);

}
