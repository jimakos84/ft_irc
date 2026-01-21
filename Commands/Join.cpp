
#include "Join.hpp"
#include "ParentCommand.hpp"
#include "../Client.hpp"
#include "../Server.hpp"
#include "../Channel.hpp"

Join::Join() {}

Join::~Join() {}

bool Join::cmdNeedsRegistration() const {
    return (true);
}

bool Join::addClientToChannel(Server *server, Channel &channel, Client &client) {
	channel.printChannelName(server, client, channel.getChannelName());
	if (channel.getInviteOnly() == true) {
		if (channel.addInvitedClient(&client, client.getNick()) == ALREADY_MEMBER)
			return (server->sendErrorMsg(client, ERR_USERONCHANNEL, client.getNick() + " " + channel.getChannelName() + " " + " :is already on channel"), false);
	}

	if (channel.getUserLimit() != 0 && channel.getMemberAmount() == channel.getUserLimit()) {
		return (server->sendErrorMsg(client, ERR_CHANNELISFULL, channel.getChannelName() + " " + " ::Cannot join channel (+l)"), false);
	}

	//if all successful add and update client & channel members
	client.addJoinedChannel(channel.getChannelName());
	channel.addClientToMemberList(&client);
	channel.removeClientFromInvited(client.getNick());
	return (true);
}

void Join::ChannelReplyMsg(Server *server, Client &client, Channel &channel) const {

	std::string  Join_msg = ":" + client.getClientFullIdentifier() + " JOIN " + channel.getChannelName() + "\r\n";

	const std::set<Client*>& members = channel.getMembers();
	for (Client* member : members)
    {
        // send to everyone except the joining client if needed
        if (member != &client)
            member->sendMsg(Join_msg);
    }

	if (channel.getTopic() == "")
		server->sendReplyMsg(client, RPL_NOTOPIC, channel.getChannelName() + " :No topic is set");
	else
		server->sendReplyMsg(client, RPL_TOPIC, channel.getChannelName() + " :" + channel.getTopic());


}

void Join::executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) {
    if (cmdParams.size() == 0) {
        server->sendErrorMsg(client, ERR_NEEDMOREPARAMS, "More Parameters needed for Join");
        return;
    }

	server->addNewChannel(cmdParams[0]);

	std::map<std::string, Channel> channel_list = server->getChannelList();
	for (auto it = channel_list.begin(); it != channel_list.end(); it++) {
		std::string curr_channelName = it->first;
		if (curr_channelName == cmdParams[0]) {
			if (addClientToChannel(server, it->second, client) == false)
				return;
			// if (it->second.getTopic() == "")
			// 	server->sendReplyMsg(client, RPL_NOTOPIC, it->second.getChannelName() + " :No topic is set");
			// else
			// 	server->sendReplyMsg(client, RPL_TOPIC, it->second.getChannelName() + " :" + it->second.getTopic());
			// if (ret_value == ALREADY_MEMBER)
			// 	return (server->sendErrorMsg(client, ERR_USERONCHANNEL, client.getNick() + " " + curr_channelName + " " + " :is already on channel"), void(0));
			// if (ret_value == FULL_CHANNEL)
			// 	return (server->sendErrorMsg(client, ERR_CHANNELISFULL, curr_channelName + " " + " ::Cannot join channel (+l)"), void(0));
		}
	}

	// server->addNewChannel(cmdParams[0]);
	// addClientToChannel(server, it->second, client);
	//channel doesnt exist, create and join
}
