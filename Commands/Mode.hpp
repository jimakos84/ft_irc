
#pragma once

#include "ParentCommand.hpp"

class Mode : public ParentCommand {
    public:
        Mode();
        ~Mode();
        bool cmdNeedsRegistration() const override;
        void executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) override;
};
