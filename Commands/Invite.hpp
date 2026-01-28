
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
        Client* getClientByNick(Server* server, const std::string& nickName);
        void sendInvitationMsg(Server* server, Client& client, Client& invitee, Channel* channel);
};
