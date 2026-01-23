
#pragma once

#include "ParentCommand.hpp"

class Nick : public ParentCommand {
    private:
        void checkNickandSet(Server *server, Client &client, const std::string &new_nick);

    public:
        Nick();
        ~Nick();
        bool cmdNeedsRegistration() const override;
        void executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) override;
};
