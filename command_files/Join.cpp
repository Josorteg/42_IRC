/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 14:54:15 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/30 00:09:41 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../inc/Includes.hpp"

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

bool Server::_joinExistingChannel(Client &client, Channel &channel)
{
	std::string message;
	std::set<int> listOfMembers;
	std::set<int> listOfInvited = channel.getInvited();
	std::set<int>::iterator itInvited = listOfInvited.find(client.getFd());

	if (channel.get_l() && static_cast<int>(channel.getMembers().size()) >= channel.getClientLimit())
	{
		_sendMessage(client, ERR_CHANNELISFULL(client.getNickname(),channel.getName()));
		return (false);
	}
	if (channel.get_i() && itInvited == listOfInvited.end())
	{
		_sendMessage(client, ERR_INVITEONLYCHAN(channel.getName()));
		return (false);
	}
	channel.addMember(client);

	message = ":" + client.getNickname() + "!~" + _getServername() + " JOIN " + channel.getName();
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
	return (true);
}

void Server::_joinNewChannel(Client &client, std::string channelName)
{
	Channel channel(this, channelName, client);

	_Channels.push_back(channel);
	std::string message = ":" + client.getNickname() + "!" + _getServername() + " JOIN " + channelName;
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
		int error = 0;
		if (!nameChecker(listOfChannels[i]))
		{
			_sendMessage(client, ERR_NOSUCHCHANNEL((listOfChannels[i])));
			continue;
		}
		if (_channelExists((listOfChannels[i])))
		{
			Channel& existingChannel = _getChannelbyname(listOfChannels[i]);
			if (parsedCommand.size() == 2 && existingChannel.get_k())
			{
				_sendMessage(client, ERR_BADCHANNELKEY(client.getNickname(),listOfChannels[i]));
				continue;
			}
			else if (parsedCommand.size() > 2 && ((existingChannel.get_k() && keyChannels.size() < i ) || (existingChannel._getPassword() != keyChannels[i])))
			{
				std::cout << "Channel : " << existingChannel.getName() << " Password : " << existingChannel._getPassword() << " Key sended : " << keyChannels[i] <<std::endl;
				_sendMessage(client, ERR_BADCHANNELKEY(client.getNickname(),listOfChannels[i]));
				continue;
			}
			if (!_joinExistingChannel(client, existingChannel))
				error = 1;
		}
		else
			_joinNewChannel(client, listOfChannels[i]);

		if (error == 0)
		{
			Channel& channel = _getChannelbyname(listOfChannels[i]);
			std::set<int> listOfMembers = channel.getMembers();
			std::string listOfClients = _getChannelMembersTxt(channel, " ", 1);

			message = ":" + client.getNickname() + "!" + client.getHostname() + " TOPIC " + channel.getName();
			if (!channel.getTopic().empty())
				message += channel.getTopic();
			_sendMessage(client, message);

			_sendMessage(channel, 0, RPL_NAMREPLY(_getServername(),client.getNickname(),channel.getName(),listOfClients));
			_sendMessage(channel, 0, RPL_ENDOFNAMES(channel.getName()));
		}
	}
}
