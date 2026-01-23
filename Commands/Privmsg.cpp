
#include "Privmsg.hpp"
#include "../Client.hpp"
#include "../Server.hpp"
#include "../Channel.hpp"

Privmsg::Privmsg() {}

Privmsg::~Privmsg() {}

bool Privmsg::cmdNeedsRegistration() const {
    return (true);
}

void Privmsg::ClientPrivMsg(Client &client, Client &to, const std::string &text) const {
    std::string msg = ":" + client.getClientFullIdentifier() + " PRIVMSG " + to.getNick() 
                    + " :" + text + "\r\n";
    client.sendMsg(msg);
}


void Privmsg::ChannelReplyMsg(Client &client, Channel &channel, const std::string &text) const
{
    std::string msg = ":" + client.getClientFullIdentifier()
                    + " PRIVMSG " + channel.getChannelName()
                    + " :" + text + "\r\n";

    const std::set<Client*>& members = channel.getMembers();
    for (Client* member : members)
    {
        if (member != &client)
            member->sendMsg(msg);
    }
}

static std::string joinText(const std::vector<std::string> &p, size_t start)
{
    std::string out;
    size_t i = start;
    while (i < p.size())
    {
        if (!out.empty())
            out += " ";
        out += p[i];
        i++;
    }
    return out;
}

void Privmsg::executeCmd(Server *server, Client &client, const std::vector<std::string> cmdParams)
{
    if (cmdParams.size() < 2)
    {
        server->sendErrorMsg(client, ERR_NEEDMOREPARAMS, "PRIVMSG :Not enough parameters");
        return;
    }

    std::string target = cmdParams[0];

    std::string text = joinText(cmdParams, 1);
    if (!text.empty() && text[0] == ':')
        text.erase(0, 1);

    bool onlySpaces = true;
    for (size_t i = 0; i < text.size(); i++)
    {
        if (text[i] != ' ' && text[i] != '\t')
        {
            onlySpaces = false;
            break;
        }
    }
    if (text.empty() || onlySpaces)
    {
        server->sendErrorMsg(client, ERR_NOTEXTTOSEND, "No text to send");
        return;
    }
    if (target[0] == '#') {
        std::map<std::string, Channel>& channel_list = server->getChannelList();
        std::map<std::string, Channel>::iterator it = channel_list.find(target);
        if (it == channel_list.end())
        {
            server->sendErrorMsg(client, ERR_NOSUCHCHANNEL, target + " :No such channel");
            return;
        }
        ChannelReplyMsg(client, it->second, text);
    }
    else {
        std::map<int, Client>& client_list = server->getClientList();
        std::map<int, Client>::iterator it = client_list.begin();
        while (it != client_list.end()){
            if (it->second.getNick() == target){
                ClientPrivMsg(client, it->second, text);
                return;
            }
            ++it;
        }
        server->sendErrorMsg(client, ERR_NOSUCHNICK, target + " :No such nick");
        return;
    }
}