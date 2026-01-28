
#include "Quit.hpp"
#include "ParentCommand.hpp"
#include "../Client.hpp"
#include "../Server.hpp"
#include "../Channel.hpp"

Quit::Quit() {}

Quit::~Quit() {}

bool Quit::cmdNeedsRegistration() const {
    return (false);
}

void Quit::executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) {
	std::string quit_msg = ":" + client.getClientFullIdentifier() + " QUIT";
	if (!cmdParams.empty())
		quit_msg += " :" + cmdParams[0];
	quit_msg += "\r\n";
	
	std::set<int> already_sent;
	std::set<std::string> joined_channs = client.getJoinedChannels();
	for (const std::string& chanName : joined_channs) {
		Channel *chan = server->getChannelByName(server, chanName);
		if (!chan)
			continue;

		for (Client* member : chan->getMembers()) {
        if (member != &client)
            member->sendMsg(quit_msg);
		}
	}
	for (const std::string& chanName : joined_channs) {
		Channel *chan = server->getChannelByName(server, chanName);
		if (!chan)
			continue;
		chan->removeClientFromMemberList(&client);
		client.leaveChannel(chanName);
		if (chan->getMembers().empty())
        	server->removeChannel(chan->getChannelName());
	}
}
