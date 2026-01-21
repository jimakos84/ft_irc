
#include "Mode.hpp"
#include "ParentCommand.hpp"
#include "../Client.hpp"
#include "../Server.hpp"
#include "../Channel.hpp"

Mode::Mode() {}

Mode::~Mode() {}

bool Mode::cmdNeedsRegistration() const {
	return (true);
}

void Mode::handleUserMode(Server *server, Client &client, const std::vector<std::string> &cmdParams) {
	for (const auto& [_, existing_client] : server->getClientList()) {
		std::string existing_nick = existing_client.getNick();
		if (existing_nick == cmdParams[0] && existing_nick == client.getNick())
			return (server->sendReplyMsg(client, RPL_UMODEIS, "+"));
	}
	server->sendErrorMsg(client, ERR_USERSDONTMATCH, ":Cannot change mode for other users");
}

bool validate_ModeStr(std::string mode_line) {
	for (char c : mode_line) {
		if (c != 'i' && c != 't' && c != 'k' && c != 'l' && c != 'o' && c != '+' && c != '-')
			return (false);
	}
	return (true);
}

void broadcast_mode_msg(Client &client, Channel &channel) {
	std::string reply = ":" + client.getClientFullIdentifier() + " MODE "
		+ channel.getChannelName() + " " + channel.getChannelmode() + "\r\n";
	const std::set<Client*>& members = channel.getMembers();
	for (Client * inform : members) 
		inform->sendMsg(reply);
}

void Mode::processModeParams(Server *server, Client &client, Channel &channel, const std::vector<std::string> &cmdParams) {
	if (validate_ModeStr(cmdParams[1]) == false)
		return (server->sendErrorMsg(client, ERR_UNKNOWNMODE , cmdParams[1] + " :is unknown mode char to me for " + channel.getChannelName()), void(0));
	if (cmdParams[1][1] == 'i') {
		if (cmdParams[1][0] == '+')
			channel.setInviteOnly(true);
		else if (cmdParams[1][0] == '-')
			channel.setInviteOnly(false);
	}
	else if (cmdParams[1][1] == 't') {
		if (cmdParams[1][0] == '+')
			channel.setTopicRestriction(true);
		else if (cmdParams[1][0] == '-')
			channel.setTopicRestriction(false);
	}
	else if (cmdParams[1][1] == 'k') {
		if (cmdParams[1][0] == '+') {
			if (cmdParams.size() > 2) {
				if (channel.getHasKey() == true)
					return (server->sendErrorMsg(client, ERR_KEYSET, channel.getChannelName() + " :Channel key already set"), void(0));
				channel.setKey(true, cmdParams[2]);
			}
			else
				return (server->sendErrorMsg(client, ERR_NEEDMOREPARAMS, "More Parameters needed for Key Setting"), void(0));
		}
		else if (cmdParams[1][0] == '-')
			channel.setKey(false, "");
	}
	else if (cmdParams[1][1] == 'l') {
		if (cmdParams[1][0] == '+') {
			if (cmdParams.size() > 2)
				channel.setUserLimit(std::stol(cmdParams[2]));
			else
				return (server->sendErrorMsg(client, ERR_NEEDMOREPARAMS, "More Parameters needed for Limit Setting"), void(0));
		}
		else if (cmdParams[1][0] == '-')
			channel.setUserLimit(0);
	}
	else if (cmdParams[1][1] == 'o') {
		if (channel.isClientOperator(&client) == false)
			return (server->sendErrorMsg(client, ERR_CHANOPRIVSNEEDED , cmdParams[0] + " :You're not channel operator"), void(0));
		if (cmdParams[1][0] == '+') {
			if (cmdParams.size() > 1) {
				if (channel.isClientMember(&client) == true)
					channel.addClientToOperatorList(&client);
				else
					return (server->sendErrorMsg(client, ERR_USERNOTINCHANNEL, client.getNick() + " " + cmdParams[0] + " :They aren't on that channel"), void(0));
			}
			else
				return (server->sendErrorMsg(client, ERR_NEEDMOREPARAMS, "More Parameters needed for Operator Setting"), void(0));
		}
		else if (cmdParams[1][0] == '-')
			channel.addClientToOperatorList(&client);
	}
	broadcast_mode_msg(client, channel);
}

void Mode::handleChannelMode(Server *server, Client &client, const std::vector<std::string> &cmdParams) {
	for (auto& [channel_name, channel] : server->getChannelList()) {
		if ("#" + channel_name == cmdParams[0]) {
			if (cmdParams.size() == 1)
				return (server->sendReplyMsg(client, RPL_CHANNELMODEIS, cmdParams[0] + " +" + channel.getChannelmode()));
			else {
				if (channel.isClientOperator(&client) == false && cmdParams[1][1] != 'o')
					return (server->sendErrorMsg(client, ERR_CHANOPRIVSNEEDED , cmdParams[0] + " :You're not channel operator"), void(0));
				return (processModeParams(server, client, channel, cmdParams), void(0));
			}
		}
	}
	server->sendErrorMsg(client, ERR_NOSUCHCHANNEL , cmdParams[0] + " :No such channel");
}

void Mode::executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) {
	if (cmdParams.size() == 0) {
		server->sendErrorMsg(client, ERR_NEEDMOREPARAMS, "More Parameters needed for Mode");
		return;
	}

	for (unsigned long i = 0; i < cmdParams.size(); i++) {
		std::cout << "Param[" << i << "]: " << cmdParams[i] << std::endl;
	}

	if (cmdParams[0][0] == '#')
		handleChannelMode(server, client, cmdParams);
	else
		handleUserMode(server, client, cmdParams);
}



/*
	MODE command
	├── validate target
	├── check permissions
	├── if no mode string → reply with current modes
	├── parse mode string (+/-)
	├── apply per-mode
	├── broadcast MODE message

	Buffer: MODE gboggion +t
Param[0]: gboggion
Param[1]: +t

	Buffer: MODE #lol +i
Param[0]: #lol
Param[1]: +i


*/
