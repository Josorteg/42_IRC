/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 14:54:15 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/05 19:27:42 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Includes.hpp"

static bool	nameChecker(std::string name)
{
	std::string chars ="&#+!";
	std::size_t found = chars.find_first_not_of(name[0]);
	if (found == std::string::npos || name.size() > 51)
		return false;
	name.erase(name.begin());
	found = name.find_first_not_of(NICK_CHARACTERS);
	if (found != std::string::npos)
		return false;
	return true;
}

void Server::_joinExistingChannel(Client &client, Channel &channel)
{
	std::set<int> listOfMembers = channel.getMembers();
	std::string message;
	std::cout<<"CHANNEL: "<<channel.getName()<<"CHECKING I state"<<"BOOLEAN: "<<channel.get_i()<<std::endl;
	std::set<int> listOfMembers2 = channel.getInvited();

	 for (std::set<int>::iterator it = listOfMembers2.begin(); it != listOfMembers2.end(); ++it) {
        std::cout << "Value: " << *it << std::endl;
    }
	std::cout << "End of values in invitedclient: " << std::endl;

	for (std::set<int>::iterator it = listOfMembers2.begin(); it != listOfMembers2.end(); ++it)
	{
		int memberFd= *it;
		 std::cout << *it << " ";
		for(std::map<int,Client>::iterator its = _Clients.begin(); its != _Clients.end(); its++)
		{
			if (its->second.getFd() == memberFd)
			{
				std::cout<<"Client in channel :"<<channel.getName()<<", user with invitation :"<<its->second.getNickname()<<std::endl;
				//send(memberFd,message.c_str(),message.size(),0);
			}
		}
	}




	std::cout<<"first cond: "<<*(channel.getInvited().find(client.getFd()))<< "second cond :"<<*(channel.getInvited().end())<<std::endl;
	if(channel.getInvited().find(client.getFd()) == channel.getInvited().end())
		std::cout<<"CHANNEL: DIDNT FOUND CLIENT IN INVITED" <<std::endl;
	else if (channel.getInvited().find(client.getFd()) != channel.getInvited().end())
			std::cout<<"CHANNEL: FOUND CLIENT IN INVITED" <<std::endl;


	// Example: find if the set contains the value 6
    int valueToFind = client.getFd();
    std::set<int>::iterator it = listOfMembers2.find(client.getFd());

    if (it != listOfMembers2.end()) {
        // Found the value in the set
        std::cout << "Found " << valueToFind << " in the set.\n";
    } else {
        // Didn't find the value in the set
        std::cout << valueToFind << " not found in the set.\n";
    }

	std::set<int> listOfInvited = channel.getInvited();
	std::set<int>::iterator itInvited = listOfInvited.find(client.getFd());
	if (channel.get_l() && static_cast<int>(channel.getMembers().size()) == channel.getClientLimit())
	//if (channel.get_i() && channel.getInvited().find(client.getFd()) == channel.getInvited().end())
	if (channel.get_i() && itInvited == listOfInvited.end())
		return(_sendMessage(client, ERR_INVITEONLYCHAN(channel.getName())));
	channel.addMember(client);

	message = ":" + client.getNickname() + "!~" + getServername() + " JOIN " + channel.getName();
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
}

void Server::_joinNewChannel(Client &client, std::string channelName)
{
	Channel channel(this, channelName, client);

	_Channels.push_back(channel);
	std::string message = ":" + client.getNickname() + "!" + getServername() + " JOIN " + channelName;
	return(_sendMessage(channel,0, message));
}

void Server::_joinServer(Client &client, std::vector<std::string> parsedCommand)
{
   std::string message;

   if (parsedCommand.size() < 2)
		return(_sendMessage(client, ERR_NEEDMOREPARAMS(parsedCommand[0])));
	std::vector<std::string> listOfChannels = _splitString(parsedCommand[1], ',');
	std::vector<std::string> keyChannels;
	if (parsedCommand.size() == 3)
		keyChannels = _splitString(parsedCommand[2], ',');
	for (size_t i = 0; i < listOfChannels.size(); ++i)
	{
		if (!nameChecker(listOfChannels[i]))
		{
			_sendMessage(client, ERR_NOSUCHCHANNEL((listOfChannels[i]))); //its correct errror?
			continue;
		}
		if (_channelExists((listOfChannels[i])))
		{
			Channel& existingChannel = _getChannelbyname(listOfChannels[i]);
			std::cout<<"BEFORE CHECKING KEY"<<std::endl;
			if (existingChannel.get_k() && (keyChannels.size() < i || existingChannel.getPassword() != keyChannels[i]))
				return(_sendMessage(client, ERR_BADCHANNELKEY(listOfChannels[i])));
			std::cout<<"AFTER CHECKING KEY"<<std::endl;
			_joinExistingChannel(client, existingChannel);
			std::cout<<"AFTER JOINING CHANNEL"<<std::endl;

		}
		else
			_joinNewChannel(client, listOfChannels[i]);

		Channel& channel = _getChannelbyname(listOfChannels[i]);
		std::set<int> listOfMembers = channel.getMembers();
		std::string listOfClients = _getChannelMembersTxt(channel, " ", 1);

		if (channel.getTopic().empty())
			_sendMessage(client, RPL_NOTOPIC(getServername(),(channel.getName())));
		else
			_sendMessage(client, RPL_TOPIC(getServername(),channel.getName(),channel.getTopic()));

		//void _sendMessage(Channel &channel,int clientFdException, std::string message);
		_sendMessage(channel,0, RPL_NAMREPLY(getServername(),client.getNickname(),channel.getName(),listOfClients));
		_sendMessage(channel,0, RPL_ENDOFNAMES(channel.getName()));
	}
}
