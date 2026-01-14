
#include "TestCmd.hpp"

TestCmd::TestCmd() {}

TestCmd::~TestCmd() {}
         
bool TestCmd::cmdNeedsRegistration() const override {
    return (false);
}

void TestCmd::executeCmd(Server &server, Client &client, const std::vector<std::string> cmdParams) {
    client.send("test command running correctly");
}