
#pragma once

#include "ParentCommand.hpp"

class Channel;

class Join : public ParentCommand {
    public:
        Join();
        ~Join();
        bool cmdNeedsRegistration() const override;
        void executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) override;

		bool addClientToChannel(Server *server, Channel &channel, Client &client);
		void ChannelReplyMsg(Server *server, Client &client, Channel &channel) const;
};
