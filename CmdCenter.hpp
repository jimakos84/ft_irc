
#pragma once

#include <string>
#include <map>
#include <memory>
#include "./Commands/ParentCommand.hpp"
#include "./Commands/Pong.hpp"

class CmdCenter {
    private:
        std::map<std::string, std::unique_ptr<ParentCommand>> _commands;

    public:
        CmdCenter();
        ~CmdCenter();

        ParentCommand* getCmd(const std::string &cmd_name) const;
};
