
#pragma once

#include "ParentCommand.hpp"

class Channel;

class Privmsg : public ParentCommand {
    public:
        Privmsg();
        ~Privmsg();
        bool cmdNeedsRegistration() const override;
        void executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) override;

        void ChannelReplyMsg(Server *server, Client &client, Channel &channel) const;
};
