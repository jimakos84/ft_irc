
#pragma once

#include "ParentCommand.hpp"

class Channel;

class Privmsg : public ParentCommand {
    public:
        Privmsg();
        ~Privmsg();
        bool cmdNeedsRegistration() const override;
        void executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) override;
        void ChannelReplyMsg(Client &client, Channel &channel, const std::string &text) const;
        void ClientPrivMsg(Client &client, Client &to, const std::string &text) const;
};
