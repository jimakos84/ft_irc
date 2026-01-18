
#pragma	once

#include <iostream>
#include <set>
#include "Client.hpp"

class Channel {
	private:
		std::string		_name;
		std::string 	_topic;
		std::string 	_channel_key;

		// Modes
		bool		_inviteOnly;
		bool		_topicSet;
		size_t		_userLimit;

		std::set<Client*> _members;
		std::set<Client*> _operators;
		std::set<std::string>	_invitedClients; //VIPs <sunglasses emoji>


	public:
		Channel(std::string given_name);
		~Channel();

		//Set functions
		// bool setInviteOnly(bool status);

		//Get functions
		std::string		getChannelName() const;
		bool 			getInviteOnly() const;
		size_t 			getUserLimit() const;
		std::string 	getTopic() const;
		const std::set<Client*>& getMembers() const;

		//other things, this and that
		int 		addInvitedClient(Client *client, std::string VIP);
		void 		removeClientFromInvited(std::string NIP);
		void		addClientToMemberList(Client *client);
		size_t 		getMemberAmount() const;
};
