
#pragma once

#include "ParentCommand.hpp"

class Channel;
class Client;

class Invite : public ParentCommand {
    public:
        Invite();
        ~Invite();
        bool cmdNeedsRegistration() const override;
        void executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) override;

		bool addClientToChannel(Server *server, Channel &channel, Client &client);
		void ChannelReplyMsg(Server *server, Client &client, Channel &channel) const;
        // bool getClientByNick(Server* server, const std::string& nickName);
        Client* getClientByNick(Server* server, const std::string& nickName);
        Channel* getChannelByName(Server *server, const std::string& name);
        void sendInvitationMsg(Server* server, Client& client, Client& invitee, Channel* channel);
};
