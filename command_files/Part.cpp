/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 17:40:34 by josorteg          #+#    #+#             */
/*   Updated: 2024/05/30 00:15:59 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"./../inc/Includes.hpp"

void Server::_partServer(Client &client, std::vector<std::string> parsedCommand)
{
	/* PART <channel> *( "," <channel> ) [ <Part Message> ]*/

	std::string message;

	if (parsedCommand.size() < 2 || (parsedCommand.size() > 2 && parsedCommand[2][0] != ':'))
		return(_sendMessage(client, ERR_NEEDMOREPARAMS(parsedCommand[0])));

	std::vector<std::string> listOfChannels = _splitString(parsedCommand[1], ',');
	std::string reason = "";

	if (parsedCommand.size() > 2)
		reason = parsedCommand[2];
	else
		reason = ":left channel";

	for (size_t i = 0; i < listOfChannels.size(); ++i)
	{
		if (!_channelExists(listOfChannels[i]))
		{
			_sendMessage(client, ERR_NOSUCHCHANNEL(parsedCommand[0]));
			continue;
		}

		Channel& channel = _getChannelbyname(listOfChannels[i]);
		if (!channel.isMember(client.getFd()))
			return(_sendMessage(client, ERR_NOTONCHANNEL(_getServername(), listOfChannels[i])));

		message = ":" + client.getNickname() + "!~" + client.getHostname() + " PART " + channel.getName() + " " + reason;
		_sendMessage(channel,0,message);
		
		channel.removeMember(client.getFd());
		channel.removeOperator(client.getFd());
		channel.removeInvited(client.getFd());
	}
}
