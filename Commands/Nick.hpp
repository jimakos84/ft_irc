
#pragma once

#include "ParentCommand.hpp"

class Nick : public ParentCommand {
    private:
        void changeNick(Server *server, Client &client_requesting_change, std::string new_nick);

    public:
        Nick();
        ~Nick(); 
        bool cmdNeedsRegistration() const override;
        void executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) override;
};