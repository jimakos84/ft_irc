
#include "ParentCommand.hpp"
#include "../Client.hpp"
#include "../Server.hpp"

ParentCommand::ParentCommand() {}

ParentCommand::~ParentCommand() {}

bool ParentCommand::isPassSet(Client &client) const {
    return (client.hasPass());
}
