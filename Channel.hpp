
#pragma	once

#include <iostream>
#include <set>
#include "Client.hpp"
#include "Server.hpp"

class Server;

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
		bool 			getInviteOnlyStatus() const;
		size_t 			getUserLimit() const;
		std::string 	getTopic() const;
		bool 			getHasKey() const;
		std::string		getKeyValue() const;
		std::string		getChannelmode();
		const std::set<Client*>& getMembers() const;

		//other things, this and that
		int 		addInvitedClient(Client *client, std::string VIP);
		void 		removeClientFromInvited(std::string NIP);
		void		addClientToMemberList(Client *client);
		void		removeClientFromMemberList(Client *client);
		size_t 		getMemberAmount() const;
		void 		addClientToOperatorList(Client *client);
		void 		removeClientFromOperatorList(Client *client);
		bool 		isClientOperator(Client *wanted_client);
		bool 		isClientMember(Client *wanted_client);
		bool		isClientInvited(std::string wanted_client);
		Client*		getMemberByNick(std::string wanted_nick);
		void		printChannelName(Server* server, Client& client, std::string name); //debug delete
		bool 		isValidChannelName(std::string name);
		bool		isExistingChannel(Server* server, std::string& channelName);
		bool 		isInviteOnly() const;
};
