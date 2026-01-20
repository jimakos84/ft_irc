
#pragma once

//IRC Protocol Reply Codes
#define RPL_WELCOME		"001"
#define RPL_YOURHOST	"002"
#define RPL_CREATED		"003"
#define RPL_NOTOPIC		"331"
#define RPL_TOPIC		"332"
#define RPL_INVITING	"341"

//ERROR codes definition
#define ERR_NOSUCHNICK          "401"
#define ERR_NOSUCHSERVER		"402"
#define ERR_UNKNOWNCOMMAND  	"421"
#define ERR_NICKNAMEINUSE   	"433"
#define ERR_USERONCHANNEL		"443"
#define ERR_NOTREGISTERED   	"451"
#define ERR_NEEDMOREPARAMS  	"461"
#define ERR_ALREADYREGISTRED	"462"
#define ERR_PASSWDMISMATCH  	"464"
#define ERR_CHANNELISFULL		"471"


//ERRRORS
#define SUCCESS			2
#define ALREADY_MEMBER	3
#define FULL_CHANNEL	4
