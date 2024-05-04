/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 14:54:15 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/04 18:32:50 by mmoramov         ###   ########.fr       */
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
	
	if (channel.get_l() && static_cast<int>(channel.getMembers().size()) == channel.getClientLimit())
		return(_sendMessage(client, ERR_CHANNELISFULL(getServername(),channel.getName())));
	if (channel.get_i() && channel.getInvited().find(client.getFd()) != channel.getInvited().end())
		return(_sendMessage(client, ERR_INVITEONLYCHAN(channel.getName())));
	channel.addMember(client);

	message = ":" + client.getNickname() + "!" + getServername() + " JOIN " + channel.getName();
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
			if (existingChannel.get_k() && (keyChannels.size() < i || existingChannel.getPassword() != keyChannels[i]))
				return(_sendMessage(client, ERR_BADCHANNELKEY(listOfChannels[i])));
			_joinExistingChannel(client, existingChannel);
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
		_sendMessage(client, RPL_NAMREPLY(getServername(),client.getNickname(),channel.getName(),listOfClients));
		_sendMessage(client, RPL_ENDOFNAMES(channel.getName()));
	}
}