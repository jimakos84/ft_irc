
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

	//implement



}
