
#include "Channel.hpp"

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
