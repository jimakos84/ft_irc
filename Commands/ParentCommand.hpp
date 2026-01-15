

#pragma once

#include <iostream>
#include <vector>

class Server;
class Client;

class ParentCommand {
    public:
        ParentCommand();
        virtual ~ParentCommand();

        virtual bool cmdNeedsRegistration() const = 0;
        virtual void executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) = 0;
};