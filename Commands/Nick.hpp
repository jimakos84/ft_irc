
#pragma once

#include "ParentCommand.hpp"
#include "../Channel.hpp"

class Nick : public ParentCommand {
    private:
        void checkNickandSet(Server *server, Client &client, const std::string &new_nick);
        void broadcastToChannels(Server *server, Client &client, std::string msg);

    public:
        Nick();
        ~Nick();
        bool cmdNeedsRegistration() const override;
        void executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) override;
};
