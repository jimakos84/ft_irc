
#include "Pong.hpp"
#include "ParentCommand.hpp"
#include "../Client.hpp"
#include "../Server.hpp"

Pong::Pong() {}

Pong::~Pong() {}
         
bool Pong::cmdNeedsRegistration() const {
    return (true);
}

void Pong::executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) {
    (void)server;
  


    if (cmdParams.size() > 0 && cmdParams[0].empty())
        return (client.send(":" + std::string("ft_irc.sadCats.fi ") + ERR_NEEDMOREPARAMS + "More Parameters needed\r\n"), void(0));
    if (cmdParams[0] != server->getServerName())
         return (client.send(":" + std::string("ft_irc.sadCats.fi ") + ERR_NOSUCHSERVER + "No such server exists\r\n"), void(0));
    client.send(":" + server->getServerName() + " PONG " + server->getServerName());
    client.send("YASSSSSS");
    return;
}