
#pragma once

#include "utils.hpp"
#include <iostream>
#include <sys/socket.h>  // for send()
#include <set>
#include "MacrosDefinitions.hpp"

class Client {
	private:
		int _fd;				//client socket file descriptor
		std::string _hostname;
		std::string _nickname;
		std::string _realname;
		std::string _username;

		std::set<std::string> _joined_channels;	//implement
		std::string _buffer;
		bool _isRegistered;
		// bool _isOperator; //implement
		bool _hasPass;
		bool _hasNick;
		bool _hasUser;



	public:
		Client(int fd, std::string host);

		void appendtoClientBuffer(std::string incoming_data);
		bool completeMessageExists();
		std::string extractMessage();

		bool isRegistered() const;
		bool hasPass();

		//Registration specific functions
		void setPass();
		void setNick(const std::string &nick);
		void setUser(const std::string &user);
		void setReal(const std::string &real);
		void setRegistered();
		void sendWelcomeMsg();

		//Get functions
		int 		getFd() const;
		std::string getNick() const;
		std::string getReal() const;
		std::string getUser() const;
		std::string	getClientFullIdentifier() const;


		//message formatting for the server reply
		void sendMsg(const std::string &msg);
		void addJoinedChannel(std::string channel_joined);

};
