
#pragma once

#include "ParentCommand.hpp"

class Channel;

class Mode : public ParentCommand {
    public:
        Mode();
        ~Mode();
        bool cmdNeedsRegistration() const override;
        void executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) override;

        void handleUserMode(Server *server, Client &client, const std::vector<std::string> &cmdParams);
        void handleChannelMode(Server *server, Client &client, const std::vector<std::string> &cmdParams);
        void processModeParams(Server *server, Client &client, Channel &channel, const std::vector<std::string> &cmdParams);
};
