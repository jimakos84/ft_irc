
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

void broadcast_mode_msg(Client &client, Channel &channel, std::string &appliedModes, std::vector<std::string> &appliedParams) {
	if (appliedModes.empty())
        return;
	std::string reply = ":" + client.getClientFullIdentifier() + " MODE " + channel.getChannelName()
						+ " " + appliedModes;

	for (const std::string &p : appliedParams)
        reply += " " + p;

	reply += "\r\n";

	const std::set<Client*>& members = channel.getMembers();
	for (Client * inform : members)
		inform->sendMsg(reply);
}

bool processKeyMode(Server *server, Client &client, Channel &channel, const std::vector<std::string> &cmdParams, std::string &appliedModes, std::vector<std::string> &appliedParams, const char &sign, size_t paramIndex) {
	if (sign == '+') {
		if (paramIndex >= cmdParams.size())
			return (server->sendErrorMsg(client, ERR_NEEDMOREPARAMS, "More Parameters needed for Key Setting"), false);

    	if (channel.getHasKey())
            return (server->sendErrorMsg(client, ERR_KEYSET, channel.getChannelName() + " :Channel key already set"), false);

        channel.setKey(true, cmdParams[paramIndex]);
        appliedParams.push_back(cmdParams[paramIndex++]);
	}
    else
		channel.setKey(false, "");
	appliedModes += 'k';
	return (true);
}

bool processLimitMode(Server *server, Client &client, Channel &channel, const std::vector<std::string> &cmdParams, std::string &appliedModes, std::vector<std::string> &appliedParams, const char &sign, size_t paramIndex) {
	if (sign == '+')
	{
		if (paramIndex >= cmdParams.size())
			return (server->sendErrorMsg(client, ERR_NEEDMOREPARAMS, "More Parameters needed for Limit Setting"), false);
		channel.setUserLimit(std::stol(cmdParams[paramIndex]));
        appliedParams.push_back(cmdParams[paramIndex++]);
	}
	else
		channel.setUserLimit(0);
	appliedModes += 'l';
	return (true);
}

bool processOperatorMode(Server *server, Client &client, Channel &channel, const std::vector<std::string> &cmdParams, std::string &appliedModes, std::vector<std::string> &appliedParams, const char &sign, size_t paramIndex) {
	if (!channel.isClientOperator(&client))
		return (server->sendErrorMsg(client, ERR_CHANOPRIVSNEEDED, channel.getChannelName() + " :You're not channel operator"), false);
	if (paramIndex >= cmdParams.size())
		return (server->sendErrorMsg(client, ERR_NEEDMOREPARAMS, "More Parameters needed for Operator Setting"), false);

	Client *target = channel.getMemberByNick(cmdParams[paramIndex]);
	if (!target || !channel.isClientMember(target))
		return (server->sendErrorMsg(client, ERR_USERNOTINCHANNEL, cmdParams[paramIndex] + " " + channel.getChannelName()
                    + " :They aren't on that channel"), false);

    if (sign == '+')
		channel.addClientToOperatorList(target);
	else
		channel.removeClientFromOperatorList(target);

	appliedParams.push_back(cmdParams[paramIndex++]);
	appliedModes += 'o';
	return (true);
}


void Mode::processModeParams(Server *server, Client &client, Channel &channel, const std::vector<std::string> &cmdParams) {
	if (cmdParams.size() < 2)
		return server->sendErrorMsg(client, ERR_NEEDMOREPARAMS, "MODE");

    const std::string &modeStr = cmdParams[1];
    if (!validate_ModeStr(modeStr)) {
        return server->sendErrorMsg(client, ERR_UNKNOWNMODE, modeStr + " :is unknown mode char to me for " + channel.getChannelName());
	}

	char sign = 0;
    size_t paramIndex = 2;

    std::string appliedModes;
    std::vector<std::string> appliedParams;

	for (size_t i = 0; i < modeStr.size(); ++i) {
		char curr_char = modeStr[i];
		if (curr_char == '+' || curr_char == '-') {
			sign = curr_char;
			appliedModes += curr_char;
			continue;
		}
		if (curr_char == 'i') {
			channel.setInviteOnly(sign == '+');
			appliedModes += curr_char;
		}
		else if (curr_char == 't') {
			channel.setTopicRestriction(sign == '+');
            appliedModes += curr_char;
		}
		else if (curr_char == 'k') {
			if (processKeyMode(server, client, channel, cmdParams, appliedModes, appliedParams, sign, paramIndex) == false)
				return;
		}
		else if (curr_char == 'k') {
			if (processLimitMode(server, client, channel, cmdParams, appliedModes, appliedParams, sign, paramIndex) == false)
				return;
		}
		else if (curr_char == 'k') {
			if (processOperatorMode(server, client, channel, cmdParams, appliedModes, appliedParams, sign, paramIndex) == false)
				return;
		}
	 }
	 broadcast_mode_msg(client, channel, appliedModes, appliedParams);
}


void Mode::handleChannelMode(Server *server, Client &client, const std::vector<std::string> &cmdParams) {
	auto it = server->getChannelList().find(cmdParams[0]);
	if (it == server->getChannelList().end())
		return (server->sendErrorMsg(client, ERR_NOSUCHCHANNEL , cmdParams[0] + " :No such channel"), void(0));

	Channel &channel = it->second;
	if (cmdParams.size() == 1)
		return (server->sendReplyMsg(client, RPL_CHANNELMODEIS, cmdParams[0] + " +" + channel.getChannelmode()));

	if (channel.isClientOperator(&client) == false)
			return (server->sendErrorMsg(client, ERR_CHANOPRIVSNEEDED , cmdParams[0] + " :You're not channel operator"), void(0));

	processModeParams(server, client, channel, cmdParams);
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
