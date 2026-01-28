
#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <map>
#include <poll.h>
#include <csignal>
#include <arpa/inet.h>
#include "Client.hpp"
#include "utils.hpp"
#include "MacrosDefinitions.hpp"
#include "CmdCenter.hpp"
#include "Channel.hpp"


class Server
{
	private:
		void    acceptClient();
		void    receiveFromClient(int fd);

		std::string                     _serverName;
		int                             _port;
		std::string                     _password;
		int                             _listenFd;

		std::vector<pollfd>             _pollFds;
		std::map<int, Client>           _clients;
		std::map<std::string, Channel>  _channels;
		CmdCenter                       _commandList;

	public:
		Server(int  port, const std::string& password);
		~Server();

		void    start();
		void    run();
		bool    commandExecute(Client &client, std::string cmd);


		//get functions
		std::string                             getServerName() const;
		std::string                             getPass() const;
		std::map<int, Client>&                  getClientList();
		const std::map<int, Client>&            getClientList() const;
		std::map<std::string, Channel>&         getChannelList();
		const std::map<std::string, Channel>&   getChannelList() const;
		Channel*								getChannelByName(Server *server, const std::string& name);

		//messagging
		void sendErrorMsg(Client &client, std::string err_code, const std::string err_msg);
		void sendReplyMsg(Client &client, std::string code, const std::string &msg);
		void sendErrNicknameInUse(Client &client, const std::string &attemptedNick);

		//Command functions 
		void 	addNewChannel(std::string channel_Name, Client &client);
		void    removeClient(int fd);
		void    removeChannel(std::string channel_name);
};
