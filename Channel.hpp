
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
		bool		_isTopicRestricted;
		bool		_hasKey;
		size_t		_userLimit;

		std::set<Client*> 		_members;
		std::set<Client*> 		_operators;
		std::set<std::string>	_invitedClients; //VIPs <sunglasses emoji>


	public:
		Channel(std::string given_name);
		~Channel();

		//Set functions
		void setInviteOnly(bool status);
		void setTopic(std::string new_topic);
		void setTopicRestriction(bool status);
		void setKey(bool status, std::string new_key);
		void setUserLimit(size_t new_limit);

		//Get functions
		std::string		getChannelName() const;
		bool 			getInviteOnly() const;
		size_t 			getUserLimit() const;
		std::string 	getTopic() const;
		bool 			getHasKey() const;
		std::string		getChannelmode();
		const std::set<Client*>& getMembers() const;

		//other things, this and that
		int 		addInvitedClient(Client *client, std::string VIP);
		void 		removeClientFromInvited(std::string NIP);
		void		addClientToMemberList(Client *client);
		size_t 		getMemberAmount() const;
		void 		addClientToOperatorList(Client *client);
		void 		removeClientToOperatorList(Client *client);
		bool 		isClientOperator(Client *client_name);
		bool 		isClientMember(Client *client_name);
};
