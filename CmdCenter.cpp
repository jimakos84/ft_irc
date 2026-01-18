
#include "CmdCenter.hpp"

CmdCenter::CmdCenter() {
	_commands["PING"] = std::make_unique<Ping>();
	_commands["PASS"] = std::make_unique<Pass>();
	_commands["NICK"] = std::make_unique<Nick>();
	_commands["USER"] = std::make_unique<User>();
	_commands["MODE"] = std::make_unique<Mode>();
	_commands["JOIN"] = std::make_unique<Join>();
}

CmdCenter::~CmdCenter() {}

ParentCommand* CmdCenter::getCmd(const std::string &cmd_name) const {
	auto it = this->_commands.find(cmd_name);
	if (it != _commands.end())
		return it->second.get();
	else
		return nullptr;
}
