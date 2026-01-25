
#include "Part.hpp"
#include "ParentCommand.hpp"
#include "../Client.hpp"
#include "../Server.hpp"

Part::Part() {}

Part::~Part() {}

bool Part::cmdNeedsRegistration() const {
    return (true);
}

Channel* Part::getChannelByName(Server *server, const std::string& name)
{
    std::map<std::string, Channel>& channel_list = server->getChannelList();
    auto it = channel_list.find(name);
    if (it != channel_list.end())
    {
        return (&it->second);
    }
    return (nullptr);
}

std::vector<std::string> splitPartLine(const std::string &line, char delim) {
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

void Part::executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams) {
    if (cmdParams.size() == 0) {
        server->sendErrorMsg(client, ERR_NEEDMOREPARAMS, "More Parameters needed for Part");
        return;
    }
    std::set<int> already_sent;
    std::vector<std::string> channels = splitPartLine(cmdParams[0], ',');
    for (std::string chans : channels) {
        if (chans[0] != '#')
            return;
        std::string msg = ":" + client.getClientFullIdentifier() + " PART " + chans + "\r\n";
        client.sendMsg(msg);
        Channel *chan = getChannelByName(server, chans);
        chan->removeClientFromMemberList(&client);
        std::cout << "Members of " << chans << " after PART:" << std::endl;

        const std::set<Client*> &members = chan->getMembers();
        for (std::set<Client*>::const_iterator it = members.begin();
             it != members.end();
             ++it)
        {
            Client *other = *it;
            if (!other)
                continue;
            if (already_sent.insert(other->getFd()).second)
                    other->sendMsg(msg);
            std::cout << " - " << (*it)->getNick() << std::endl;
        }
    }
}
