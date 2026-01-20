
#include "Mode.hpp"
#include "ParentCommand.hpp"
#include "../Client.hpp"
#include "../Server.hpp"

Mode::Mode() {}

Mode::~Mode() {}

bool Mode::cmdNeedsRegistration() const {
    return (true);
}

void Mode::executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) {
    if (cmdParams.size() == 0) {
        server->sendErrorMsg(client, ERR_NEEDMOREPARAMS, "More Parameters needed for Mode");
        return;
    }

	if (cmdParams[0][0] == '#')
    {
        if (cmdParams.size() == 1)
        {
            server->sendReplyMsg(client, "324", cmdParams[0] + " +");
        }
        return ;
    }
}


