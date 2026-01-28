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

void sendInviteeMsg(Client &invitee, const std::string &msg, std::string serverName)
{
    std::string reply = ":" + serverName + " " + msg + "\r\n";
    invitee.sendMsg(reply);
}


void Invite::sendInvitationMsg(Server* server, Client& client, Client& invitee, Channel* channel)
{
    server->sendReplyMsg(client, RPL_INVITING, invitee.getNick() + " " + channel->getChannelName());
    std::string serverName = server->getServerName();
    std::string invitee_msg = client.getClientFullIdentifier() + " INVITE "  + invitee.getNick() + " :" + channel->getChannelName();
    sendInviteeMsg(invitee, invitee_msg, serverName);
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

    Channel* channel = server->getChannelByName(server, channelName);
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
    if (channel->addInvitedClient(invitee, inviteeName) == ALREADY_MEMBER)
    {
        server->sendErrorMsg(client, ERR_USERONCHANNEL, inviteeName + " is already on channel");
        return ;
    }

    sendInvitationMsg(server, client, *invitee, channel);

}
