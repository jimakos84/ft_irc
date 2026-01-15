
#pragma once

#include "ParentCommand.hpp"

class Pass : public ParentCommand {
    public:
        Pass();
        ~Pass(); 
        bool cmdNeedsRegistration() const override;
        void executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) override;
};