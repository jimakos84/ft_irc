
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

Channel* getChannelByName(Server *server, const std::string& name)
{
    std::map<std::string, Channel>& channel_list = server->getChannelList();
    auto it = channel_list.find(name);
    if (it != channel_list.end())
    {
        return (&it->second);
    }
    return (nullptr);
}

void Quit::executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) {
	std::string quit_msg = ":" + client.getClientFullIdentifier() + " QUIT";
	if (!cmdParams.empty())
		quit_msg += " :" + cmdParams[0];
	quit_msg += "\r\n";

	//delete
	std::cout << "quit_msg:: " << quit_msg << std::endl;

	std::set<int> already_sent;
	std::set<std::string> joined_channs = client.getJoinedChannels();
	for (const std::string& chanName : joined_channs) {
		Channel *chan = getChannelByName(server, chanName);
		if (!chan)
			continue;

		for (Client* member : chan->getMembers()) {
        if (member != &client)
            member->sendMsg(quit_msg);
		}
	}

	for (const std::string& chanName : joined_channs) {
		Channel *chan = getChannelByName(server, chanName);
		if (!chan)
			continue;

		chan->removeClientFromMemberList(&client);
		client.leaveChannel(chanName);
		if (chan->getMembers().empty())
        	server->removeChannel(chan->getChannelName());
	}
}
