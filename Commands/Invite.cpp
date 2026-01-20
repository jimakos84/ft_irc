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

Client* Invite::getClientByNick(Server* server, std::string nickName)
{
    std::map<std::int, Client> client_list = server->getClientList();

    for (auto it = client_list.begin(); it != client_list.end(); it++) 
    {
        if (it->second.getNick(nickName))
        {
            return (it->second);
        }
    }
    return ("");
}

Channel* Invite::getChannelByName(Server *server, std::string& name)
{
    std::map<std::string, Channel> channel_list = server->getChannelList();
    auto it = channel_list.find(name);
    if (it != channel_list.end())
    {
        return (&it->second);
    }
    return ("");
}
void Invite::executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) {
    if (cmdParams.size() <= 1) {
        server->sendErrorMsg(client, ERR_NEEDMOREPARAMS, "More Parameters needed for Join");
        return;
    }

    std::string inviteeName = cmdParams[0];
    std::string channelName = cmdParams[1];
    
    Client* invitee = getClientByNick(server, inviteeName);
    if (invitee == "")
    {
        server->sendErrorMsg(client, ERR_NOSUCHNICK, inviteeName + " :No such nick exists!");
        return ;
    }
    Channel* channel = getChannelByName(server, channelName);
    if (isOnChannel(invitee, channel))
    {
        server->sendErrorMsg(client, ERR_USERONCHANNEL, inviteeName + channelName + " is already on channel");
    }

}
