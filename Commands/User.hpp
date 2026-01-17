
#pragma once

#include "ParentCommand.hpp"

class User : public ParentCommand {
    public:
        User();
        ~User();
        bool cmdNeedsRegistration() const override;
        void executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) override;
};
