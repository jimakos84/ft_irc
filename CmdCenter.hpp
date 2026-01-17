
#pragma once

#include <string>
#include <map>
#include <memory>
#include "./Commands/ParentCommand.hpp"
#include "./Commands/Ping.hpp"
#include "./Commands/Pass.hpp"
#include "./Commands/Nick.hpp"
#include "./Commands/User.hpp"
#include "./Commands/Mode.hpp"

class CmdCenter {
    private:
        std::map<std::string, std::unique_ptr<ParentCommand>> _commands;

    public:
        CmdCenter();
        ~CmdCenter();

        ParentCommand* getCmd(const std::string &cmd_name) const;
};
