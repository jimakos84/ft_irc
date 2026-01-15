
#include "Parser.hpp"
#include <iostream>

// void Parser::handle(Client& client, const std::string& line) {
//     (void)client;
//     std::cout << "Received: " << line << std::endl;
// }

Parser::Parser() {}

Parser::~Parser() {}

void Parser::splitIrcLine(const std::string& cmd_line, std::string& cmd,
                         std::vector<std::string>& params)
{
    std::istringstream iss(cmd_line);
    std::string token;

    if (!(iss >> cmd))
        return ;

    while (iss >> token) {
        if (!token.empty() && token[0] == ':') {
            std::string trailing = token.substr(1);
            std::string rest;
            std::getline(iss, rest);
            if (!rest.empty() && rest[0] == ' ')
                rest.erase(0, 1);
            if (!rest.empty())
                trailing += " " + rest;
            params.push_back(trailing);
            break;
        }
        params.push_back(token);
    }
}