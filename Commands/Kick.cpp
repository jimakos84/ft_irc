
#include "Kick.hpp"
#include "ParentCommand.hpp"
#include "../Client.hpp"
#include "../Server.hpp"
#include "../Channel.hpp"

Kick::Kick() {}

Kick::~Kick() {}

bool Kick::cmdNeedsRegistration() const {
    return (true);
}

void kickUser(Client &client, Client &target, Channel &channel, const std::vector<std::string> &cmdParams) {
    channel.removeClientFromMemberList(&target);
    channel.removeClientFromOperatorList(&target);   //this skips if target doesnt exist right?

    //sendmsg
    std::string kick_msg = ":" + client.getClientFullIdentifier() + " KICK " + channel.getChannelName() 
    + " " + target.getNick();
    if (cmdParams.size() == 3)
        kick_msg += " :" + cmdParams[2];
    kick_msg += "\r\n";

    const std::set<Client*>& members = channel.getMembers();
	for (Client * inform : members)
		inform->sendMsg(kick_msg);
}

void Kick::executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) {
    if (cmdParams.size() == 0 || cmdParams.size() == 1)
        return (server->sendErrorMsg(client, ERR_NEEDMOREPARAMS, "More Parameters needed for Kick"), void(0));
    
    /*//DELETE
    for (unsigned long i = 0; i < cmdParams.size(); i++) {
		std::cout << "Param[" << i << "]: " << cmdParams[i] << std::endl;
	}

    std::cout << "KICK channel: [" << cmdParams[0] << "] size="
          << cmdParams[0].size() << "\n";
*/

    std::vector<std::string> kick_channels = splitJoinLine(cmdParams[0], ',');
    std::vector<std::string> users_to_kick = splitJoinLine(cmdParams[1], ',');

    if (!((kick_channels.size() == 1 && users_to_kick.size() >= 1) || kick_channels.size() == users_to_kick.size()))
        return (server->sendErrorMsg(client, ERR_NEEDMOREPARAMS, "KICK"), void(0));
    
    for (size_t i = 0; i < users_to_kick.size(); ++i) {
        const std::string &channelName = (kick_channels.size() == 1 ? kick_channels[0] : kick_channels[i]);

        auto it = server->getChannelList().find(channelName);
	    if (it == server->getChannelList().end()) {
	    	server->sendErrorMsg(client, ERR_NOSUCHCHANNEL , channelName + " :No such channel");
            continue;
        }

	    Channel &channel = it->second;
        if (channel.isClientOperator(&client) == false) {
		    server->sendErrorMsg(client, ERR_CHANOPRIVSNEEDED , channelName + " :You're not channel operator");
            continue;
        }
        if (channel.isClientMember(&client) == false) {
		    server->sendErrorMsg(client, ERR_NOTONCHANNEL , channelName + " :You're not on that channel");
            continue;
        }

        Client *target = channel.getMemberByNick(users_to_kick[i]);
	    if (!target || !channel.isClientMember(target)) {
		    server->sendErrorMsg(client, ERR_USERNOTINCHANNEL, users_to_kick[i] + " " + channel.getChannelName()
                    + " :They aren't on that channel");
            continue;
        }
        kickUser(client, *target, channel, cmdParams);
    }
}
