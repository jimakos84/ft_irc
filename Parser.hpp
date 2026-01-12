
#pragma once

#include <string>

class Client;

class Parser {
public:
    static void handle(Client& client, const std::string& line);
};