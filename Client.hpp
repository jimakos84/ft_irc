
#include "utils.hpp"
#include <iostream>
#include <sys/socket.h>  // for send()

class Client {
	private:
		int _fd;				//client socket file descriptor
		std::string _hostname;
		std::string _nickname;
		std::string _realname;
		std::string _username;
		
		// std::set<std::string> _joined_channels;
		std::string _buffer;
		bool _isRegistered;
		// bool _isOperator;
		bool _hasPass;
		bool _hasNick;
		bool _hasUser;
		
		

	public:
		Client(int fd);

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

		//Get functions
		int getFd() const;
		std::string getNick() const;
		std::string getReal() const;
		std::string getUser() const;

		//message formatting for the server reply
		void send(const std::string &msg);

};
