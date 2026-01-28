
#include "Channel.hpp"
#include "Server.hpp"

Channel::Channel(const std::string given_name) : _name(given_name) {
	_topic = "";
	_channel_key = "";
	_inviteOnly = false;
	_isTopicRestricted = false;
	_hasKey = false;
	_userLimit = 0;
}

Channel::~Channel() {}

std::string Channel::getChannelName() const {
	return (_name);
}

void Channel::setInviteOnly(bool status) {
	_inviteOnly = status;
}

void Channel::setTopic(std::string new_topic) {
	_topic = new_topic;
}

void Channel::setTopicRestriction(bool status) {
	_isTopicRestricted = status;
}

void Channel::setKey(bool status, std::string new_key) {
	_hasKey = status;
	_channel_key = new_key;
}

void Channel::setUserLimit(size_t new_limit) {
	_userLimit = new_limit;
}

bool Channel::getInviteOnlyStatus() const {
	return (_inviteOnly);
}

bool Channel::getTopicRestrictionStatus() const {
	return (_isTopicRestricted);
}

size_t Channel::getUserLimit() const {
	return (_userLimit);
}

size_t Channel::getMemberAmount() const {
	return(_members.size());
}

std::string Channel::getTopic() const {
	return(_topic);
}

bool Channel::getHasKey() const {
	return(_hasKey);
}

std::string Channel::getKeyValue() const {
	return(_channel_key);
}

const std::set<Client*>& Channel::getMembers() const {
	return (_members);
}

std::string	Channel::getChannelmode() {
	std::string channel_modes;

	if (_inviteOnly == true)
		channel_modes += "i";
	if (_isTopicRestricted == true)
		channel_modes += "t";

	if (_hasKey == true)
		channel_modes += "k";

	if (_userLimit != 0)
		channel_modes += "l";

	if (channel_modes.empty())
		return ("");

	return (channel_modes);	
}

int Channel::addInvitedClient(Client *client, std::string VIP) {
	if (_members.find(client) != _members.end()) {
		return (ALREADY_MEMBER);
	}
	_invitedClients.insert(VIP);
	return (SUCCESS);
}

void Channel::removeClientFromInvited(std::string NIP) {
	_invitedClients.erase(NIP);
}

void Channel::addClientToMemberList(Client *client) {
	_members.insert(client);
}

void Channel::removeClientFromMemberList(Client *client) {
	_members.erase(client);
}

void Channel::addClientToOperatorList(Client *client) {
	_operators.insert(client);
}

void Channel::removeClientFromOperatorList(Client *client) {
	_operators.erase(client);
}

bool Channel::isClientOperator(Client *wanted_client) {
	for (Client * curr_client : _operators) {
		if (curr_client == wanted_client)
			return (true);
	}
	return false;
}

bool Channel::isClientMember(Client *wanted_client) {
	for (Client * curr_client : _members) {
		if (curr_client == wanted_client)
			return (true);
	}
	return false;
}

bool Channel::isClientInvited(std::string wanted_client) {
	for (std::string curr_client : _invitedClients) {
		if (curr_client == wanted_client)
			return (true);
	}
	return false;
}

Client*	Channel::getMemberByNick(std::string wanted_nick) {
	for (Client * curr_client : _members) {
		if (curr_client->getNick() == wanted_nick)
			return (curr_client);
	}
	return (nullptr);
}
