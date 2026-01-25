
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
	std::string mode_params;

	if (_inviteOnly == true)
		channel_modes += "i";
	if (_isTopicRestricted == true) {
		channel_modes += "t";
		mode_params += _topic;
	}
	if (_hasKey == true) {
		channel_modes += "k";
		mode_params += " " + _channel_key;
	}
	if (_userLimit != 0) {
		channel_modes += "l";
		mode_params += " " + std::to_string(_userLimit);
	}

	if (channel_modes.empty())
		return ("");

	return (channel_modes + " " + mode_params);
}
  
//check if the channel has a valid name
bool Channel::isValidChannelName(std::string name)
{
	if (name.empty() || name[0] != '#' || name.size() > 50)
		return (false);
	for (size_t i = 1; i < name.size(); i++)
	{
		if (name[i] == ' ' || name[i] == ',' || name[i] == ':' || (name[i] < 32 || name[i] == 127) )
		return (false);
	}
	return (true);
}

//check if the channel exists already on server
bool Channel::isExistingChannel(Server* server, std::string& channelName)
{
	std::map<std::string, Channel>   channel_list = server->getChannelList();
	for (auto it = channel_list.begin(); it != channel_list.end(); it ++)
	{
		std::string curr_channelName = it->first;
		if (curr_channelName == channelName)
		{
			return (true);
		}
	}
	return (false);
}

//check if the channel is a invite-only channel
bool Channel::isInviteOnly() const
{
	bool value;
	if (_inviteOnly == true)
		value = true;
	else
		value = false;
	return (value);
}

void Channel::printChannelName(Server* server, Client& client, std::string name)
{
	if (isValidChannelName(name) == true)
	{
		server->sendReplyMsg(client, RPL_CREATED, name + ": valid channelname");
	}
	else 
	{
		server->sendErrorMsg(client, ERR_NOSUCHCHANNEL, name + " : invalid channel name");
	}
}

int Channel::addInvitedClient(Client *client, std::string VIP) {
	if (_members.find(client) != _members.end()) {
		return (ALREADY_MEMBER);
	}
	_invitedClients.insert(VIP);
	return (SUCCESS);
}

//removeInviteClient for JOIN Command
void Channel::removeClientFromInvited(std::string NIP) {
	_invitedClients.erase(NIP);
}


// MODE command:
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
