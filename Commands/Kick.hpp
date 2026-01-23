
#pragma once

#include "ParentCommand.hpp"

class Channel;

class Kick : public ParentCommand {
    private:
        std::vector<std::string> _kick_channels;
        
    public:
        Kick();
        ~Kick();
        bool cmdNeedsRegistration() const override;
        void executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) override;
};
