
#include <iostream>
#include <sys/socket.h>  // for send()

class Client {
	private:
		int _fd;				//client socket file descriptor
		std::string _nickname;
		std::string _username;
		std::string _realname;
		std::string _hostname;
		// std::set<std::string> _joined_channels;
		std::string _buffer;
		// bool _hasPass;
		bool _isRegistered;
		bool _isOperator;
	
	public:
		Client(int fd, const std::string& hostname);

		void appendtoClientBuffer(std::string incoming_data);
		bool completeMessageExists();
		std::string extractMessage();

		bool _isRegistered() const;

		//Registration specific functions
		// void setPass();
		void setNick(const std::string &nick);
		void setUser(const std::string &user, const std::string &real);
		void setRegistered();

		//message formatting for the server reply
		void send(const std::string &msg);

};