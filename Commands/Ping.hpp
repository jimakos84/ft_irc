

#pragma once

#include "ParentCommand.hpp"


class Ping : public ParentCommand {
    public:
        Ping();
        ~Ping();
        bool cmdNeedsRegistration() const override;
        void executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) override;
};
