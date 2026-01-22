
#include "Channel.hpp"
#include "Server.hpp"

Channel::Channel(const std::string given_name) : _name(given_name) {
	_topic = "";
	_channel_key = "";
	_inviteOnly = false;
	_topicSet = false;
	_userLimit = 0;
}

Channel::~Channel() {}

std::string Channel::getChannelName() const {
	return (_name);
}

//MODE ::
// bool Channel::setInviteOnly(bool status) {
// 	_inviteOnly = status;
// }

bool Channel::getInviteOnly() const {
	return (_inviteOnly);
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

const std::set<Client*>& Channel::getMembers() const {
	return (_members);
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

// void Channel::Debug() const {

// }
