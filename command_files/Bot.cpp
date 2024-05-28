/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 20:05:32 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/28 17:32:17 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../inc/Includes.hpp"

void Server::_botServer(Client &client, std::vector<std::string> parsedCommand)
{
	/*  BOT ON  #test password - ON on bot with correct password
		BOT OFF #test password - OFF bot with correct password
	*/
	std::set<int> listOfMembers;
	std::string password = "42";

	if (parsedCommand.size() < 4)
		return(_sendMessage(client, ERR_NEEDMOREPARAMS(parsedCommand[0])));
	if (parsedCommand[1] != "ON" && parsedCommand[1] != "OFF")
		return(_sendMessage(client, ERR_NEEDMOREPARAMS((parsedCommand[2]))));
	if (!_channelExists(parsedCommand[2]))
		return(_sendMessage(client, ERR_NOSUCHCHANNEL((parsedCommand[2]))));

	Channel& channel = _getChannelbyname(parsedCommand[2]);
	if (!channel.isMember(client.getFd()))
		return(_sendMessage(client, ERR_NOTONCHANNEL(_getServername(), channel.getName())));
	if (!channel.isOperator(client.getFd()))
		return(_sendMessage(client, ERR_CHANOPRIVSNEEDED(_getServername(), channel.getName())));

	if (parsedCommand[3] != password)
		return(_sendMessage(client, ERR_BOT_WRONGPASS(_getServername(), client.getNickname(), channel.getName())));

	std::string message;
	if (parsedCommand[1] == "ON")
	{
		if (channel.getBotActive() == true)
			return(_sendMessage(client, ERR_BOT_ISACTIVE(_getServername(), client.getNickname(), channel.getName())));
		channel.setBotActive(true);

		message = ":BOT!~" + _getServername() + " JOIN " + channel.getName();
		_sendMessage(channel,0, message);

		std::vector<std::string> whoParsedCommand;
		whoParsedCommand.push_back("WHO");
		whoParsedCommand.push_back(channel.getName());
		listOfMembers = channel.getMembers();
		for (std::set<int>::iterator i = listOfMembers.begin(); i != listOfMembers.end(); ++i)
		{
			int a= *i;
			std::map<int, Client>::iterator it = _Clients.find(a);
			_whoServer(it->second,whoParsedCommand);
		}

		std::string listOfClients = _getChannelMembersTxt(channel, " ", 1);
		_sendMessage(channel, 0, RPL_NAMREPLY(_getServername(),"BOT",channel.getName(),listOfClients));
		_sendMessage(channel, 0, RPL_ENDOFNAMES(channel.getName()));

		message = ":BOT!~" + client.getHostname() + " PRIVMSG " + channel.getName() + " " + ":Hello, from now bad words are forbidden. The third time you use a bad word, I will kick you";
		_sendMessage(channel, 0, message);
	}
	else if (parsedCommand[1] == "OFF")
	{
		channel.setBotActive(false);
		message = ":BOT!~" + client.getHostname() + " PART " + channel.getName() + " " + ":Bot was turned OFF";
		_sendMessage(channel, 0, message);
	}
}

int badWordSeeker(std::vector<std::string> badWordList,std::string &message)
{
    std::size_t pos = 0;
	int result = 0;

	std::cout<< "badWordSeeker message before: "<< message << std::endl;
    for (std::vector<std::string>::const_iterator it = badWordList.begin(); it != badWordList.end(); ++it)
	{
		std::string badWord = *it;
		std::string replaceWord = "*";
		pos = 0;

		std::cout<< "badWordSeeker message bad word to search: "<< badWord << std::endl;
    	while ( (pos = message.find(badWord, pos)) != std::string::npos )
		{
			result = 1;
			message.erase(pos,badWord.length());
			for (size_t i = 0; i < badWord.length(); i++)
				message.insert(pos,replaceWord);
			pos += badWord.length();
			std::cout<< "badWordSeeker message bad word found: "<< badWord << std::endl;
		}
	}
	std::cout<< "badWordSeeker message after: " << message << std::endl;
	return (result);
}

void Server::_botchecker(Client &client, Channel &channel, std::string &message)
{
	std::string reply;
	std::vector<std::string> badWordList;
	badWordList.push_back("mierda");
	badWordList.push_back("cabrón");
	badWordList.push_back("puta");
	if (!badWordSeeker(badWordList, message))
		return;
	channel.addToBotCounter(client);
	if (channel.getBadWordCounter(client) >= 3)
	{
		reply = ":BOT KICK " + channel.getName() + " " + client.getNickname() + " :USE OF BAD WORDS";
		_sendMessage(channel,0,reply);

		channel.removeMember(client.getFd());
		channel.removeOperator(client.getFd());
		channel.removeInvited(client.getFd());

		//if there is no one in the channel, delete channel
		if (channel.getMembers().size() < 1)
		{
			for (std::vector<Channel>::iterator it = _Channels.begin(); it != _Channels.end(); ++it)
			{
				if (*it == channel)
				{
					this->_Channels.erase(it);
					break;
				}
			}
		}
	}
	else if (channel.getBadWordCounter(client) == 2)
	{
		reply = ":BOT!" + client.getHostname() + " PRIVMSG " + channel.getName() + " ";
		reply += ":This is the second time you use a bad word. If you do it again, you will be kicked from the channel";
		_sendMessage(client, reply);
	}
	else if (channel.getBadWordCounter(client) == 1)
	{
		reply = ":BOT!" + client.getHostname() + " PRIVMSG " + channel.getName() + " ";
		reply += ":You are using bad words " + client.getNickname() + " its the first time you used a badword. Its forbidden";
		_sendMessage(client, reply);
	}
	std::cout<<"_botchecker: " << client.getNickname() << " " << channel.getName() << std::endl;
}

