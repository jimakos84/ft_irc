
#include "Join.hpp"
#include "../Client.hpp"
#include "../Server.hpp"
#include "../Channel.hpp"

Join::Join() {}

Join::~Join() {}

bool Join::cmdNeedsRegistration() const {
    return (true);
}

std::vector<std::string> splitJoinLine(const std::string &line, char delim) {
    std::vector<std::string> result;
    std::string current;

    for (char c : line)
    {
        if (c == delim)
        {
            result.push_back(current);
            current.clear();
        }
        else
            current += c;
    }
    result.push_back(current);
    return result;
}

bool Join::addClientToChannel(Server *server, Channel &channel, std::string sent_channel_key, Client &client) {
	if (channel.getInviteOnlyStatus() == true) {
        if (channel.isClientMember(&client) == true)
			return (server->sendErrorMsg(client, ERR_USERONCHANNEL, client.getNick() + " " + channel.getChannelName() + " " + " :is already on channel"), false);
        if (channel.isClientInvited(client.getNick()) == false)
            return (server->sendErrorMsg(client, ERR_INVITEONLYCHAN, channel.getChannelName() + " " + " :Cannot join channel (+i)"), false);
	}

	if (channel.getUserLimit() != 0 && channel.getMemberAmount() == channel.getUserLimit()) {
		return (server->sendErrorMsg(client, ERR_CHANNELISFULL, channel.getChannelName() + " " + " :Cannot join channel (+l)"), false);
	}

    if (channel.getHasKey() == true) {
        if (sent_channel_key != channel.getKeyValue())
            return (server->sendErrorMsg(client, ERR_BADCHANNELKEY, channel.getChannelName() + " " + " :Cannot join channel (+k)"), false);
    }


	client.addJoinedChannel(channel.getChannelName());
	channel.addClientToMemberList(&client);
	channel.removeClientFromInvited(client.getNick());
	return (true);
}

void Join::ChannelReplyMsg(Server *server, Client &client, Channel &channel) const
{
    // const std::set<Client*>& members = channel.getMembers();
    // std::cout << "JOIN broadcast members of " << channel.getChannelName() << ":\n";
    // for (Client* m : members)
    //     std::cout << " - " << m->getNick() << "\n";


    std::string Join_msg = ":" + client.getClientFullIdentifier()
                         + " JOIN " + channel.getChannelName() + "\r\n";

    const std::set<Client*>& members = channel.getMembers();
    for (Client* member : members)
        member->sendMsg(Join_msg);

    if (channel.getTopic().empty())
        server->sendReplyMsg(client, RPL_NOTOPIC,
                             channel.getChannelName() + " :No topic is set");
    else
        server->sendReplyMsg(client, RPL_TOPIC,
                             channel.getChannelName() + " :" + channel.getTopic());
    std::string names;
    for (Client* m : members)
    {
        if (!names.empty())
            names += " ";
        if (channel.isClientOperator(m) == true)
            names += "@" + m->getNick();
        else
            names += m->getNick();
    }

    server->sendReplyMsg(client, RPL_NAMREPLY,
                         "= " + channel.getChannelName() + " :" + names);

    server->sendReplyMsg(client, RPL_ENDOFNAMES,
                         channel.getChannelName() + " :End of /NAMES list.");
}


void Join::executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) {
    if (cmdParams.size() == 0) {
        server->sendErrorMsg(client, ERR_NEEDMOREPARAMS, "More Parameters needed for Join");
        return;
    }
    std::vector<std::string> channels = splitJoinLine(cmdParams[0], ',');
    std::vector<std::string> keys;
    if (cmdParams.size() > 1)
        keys = splitJoinLine(cmdParams[1], ',');
    for (size_t i = 0; i < channels.size(); ++i) {
        server->addNewChannel(channels[i], client);

        std::map<std::string, Channel>& channel_list = server->getChannelList();
        auto it = channel_list.find(channels[i]);
        // if (it == channel_list.end())
        //     return;
        Channel& channel = it->second;
        std::string channel_key = (i < keys.size() ? keys[i] : "");
        if (!addClientToChannel(server, channel, channel_key, client))
            return;
        ChannelReplyMsg(server, client, channel);
    }
}
