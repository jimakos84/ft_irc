
#pragma once

#include "ParentCommand.hpp"
#include "../Channel.hpp"


class Part : public ParentCommand {
    public:
        Part();
        ~Part();
        bool cmdNeedsRegistration() const override;
        void executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) override;
};
