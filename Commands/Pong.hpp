

#pragma once

#include "ParentCommand.hpp"


class Pong : public ParentCommand {
    private:
        std::string _test_msg;

    public:
        Pong();
        ~Pong(); 
        bool cmdNeedsRegistration() const override;
        void executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) override;
};