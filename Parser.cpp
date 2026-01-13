
#include "Parser.hpp"
#include "Client.hpp"
#include <iostream>

void Parser::handle(Client& client, const std::string& line) {
    (void)client;
    std::cout << "Received: " << line << std::endl;
}