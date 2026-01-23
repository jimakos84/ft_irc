
#pragma once

//IRC Protocol Reply Codes
#define RPL_WELCOME			"001"	//"Welcome to the Internet Relay Network (newline:) <nick>!<user>@<host>"
#define RPL_YOURHOST		"002"	//"Your host is <servername>, running version <ver>"
#define RPL_CREATED			"003"	//"This server was created <date>"
#define RPL_UMODEIS			"221"	//"<user mode string>"
#define RPL_CHANNELMODEIS	"324"	//"<channel> <mode> <mode params>"
#define RPL_NOTOPIC			"331"	//"<channel> :No topic is set"
#define RPL_TOPIC			"332"	//"<channel> :<topic>"
#define RPL_INVITING		"341"	//"<channel> <nick>"
#define RPL_NAMREPLY   		"353"	//<operator> <channel> : <operator> <nick> * <operator> <nick>
#define RPL_ENDOFNAMES  	"366"	//"<channel> :End of NAMES list"

//ERROR codes definition
#define ERR_NOSUCHSERVER		"402"	//"<server name> :No such server"
#define ERR_NOSUCHCHANNEL		"403"	//"<channel name> :No such channel"
#define ERR_UNKNOWNCOMMAND  	"421"	//"<command> :Unknown command"
#define ERR_NICKNAMEINUSE   	"433"	//"<nick> :Nickname is already in use"
#define ERR_USERNOTINCHANNEL	"441"	//"<nick> <channel> :They aren't on that channel"
#define ERR_NOTONCHANNEL        "442"   //"<channel> :You're not on that channel" (when client tries to performa channel cmd for which they are not member)
#define ERR_USERONCHANNEL		"443"	//"<user> <channel> :is already on channel"
#define ERR_NOTREGISTERED   	"451"	//":You have not registered"
#define ERR_NEEDMOREPARAMS  	"461"	//"<command> :Not enough parameters"
#define ERR_ALREADYREGISTRED	"462"	//:Unauthorized command (already registered)"
#define ERR_PASSWDMISMATCH  	"464"	//":Password incorrect"
#define ERR_KEYSET				"467"	//"<channel> :Channel key already set"
#define ERR_CHANNELISFULL		"471"	//"<channel> :Cannot join channel (+l)"
#define ERR_UNKNOWNMODE			"472"	//"<char> :is unknown mode char to me for <channel>"
#define ERR_INVITEONLYCHAN		"473"	//"<channel> :Cannot join channel (+i)"
#define ERR_BADCHANNELKEY		"475"	//"<channel> :Cannot join channel (+k)"
#define ERR_CHANOPRIVSNEEDED	"482"	//"<channel> :You're not channel operator"
#define ERR_USERSDONTMATCH		"502"	//":Cannot change mode for other users"


//ERRRORS
#define SUCCESS			2
#define ALREADY_MEMBER	3
#define FULL_CHANNEL	4


