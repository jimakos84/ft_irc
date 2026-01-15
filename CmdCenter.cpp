
#include "CmdCenter.hpp"

CmdCenter::CmdCenter() {
	_commands["PING"] = std::make_unique<Pong>();
}

CmdCenter::~CmdCenter() {}

ParentCommand* CmdCenter::getCmd(const std::string &cmd_name) const {
	auto it = this->_commands.find(cmd_name);
	if (it != _commands.end())
		return it->second.get();
	else
		return nullptr;
}