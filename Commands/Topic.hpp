
#pragma once

#include "ParentCommand.hpp"

class Channel;
class Client;

class Topic : public ParentCommand {
    public:
        Topic();
        ~Topic();
        bool cmdNeedsRegistration() const override;
        void executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) override;
        Channel* getChannelByName(Server *server, const std::string& name);
};