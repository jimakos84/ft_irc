
#pragma once

#include "ParentCommand.hpp"

class Channel;

class Quit : public ParentCommand {
    public:
        Quit();
        ~Quit();
        bool cmdNeedsRegistration() const override;
        void executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) override;
};
