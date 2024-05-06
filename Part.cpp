/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 17:40:34 by josorteg          #+#    #+#             */
/*   Updated: 2024/05/06 18:19:41 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes.hpp"

void Server::_partServer(Client &client, std::vector<std::string> parsedCommand)
{
    /* PART <channel> *( "," <channel> ) [ <Part Message> ]*/

    std::string message;

	if (parsedCommand.size() < 2)
		return(_sendMessage(client, ERR_NEEDMOREPARAMS(parsedCommand[0])));

	std::vector<std::string> listOfChannels = _splitString(parsedCommand[1], ',');
	std::string reason = "";
	if (parsedCommand.size() > 2)
	{
		for (size_t j = 2;j < parsedCommand.size();j++)
			reason += " " + parsedCommand[j];
	}
	else
		reason = " :left channel";

	for (size_t i = 0; i < listOfChannels.size(); ++i)
	{
		if (!_channelExists(listOfChannels[i]))
		{
			_sendMessage(client, ERR_NOSUCHCHANNEL(parsedCommand[0]));
			continue;
		}

		Channel& channel = _getChannelbyname(listOfChannels[i]);
		if (!channel.isMember(client.getFd()))
			return(_sendMessage(client, ERR_NOTONCHANNEL(getServername(), listOfChannels[i])));
		std::cout<< "PART reason" << reason << std::endl;
		message = ":" + client.getNickname() + "!~" + client.getHostname() + " PART " + channel.getName() + reason;
		std::cout<< "PART message" << message << std::endl;
		//remove client from the channel.
		_sendMessage(channel,0,message);
		channel.removeMember(client.getFd());
		channel.removeOperator(client.getFd());
		channel.removeInvited(client.getFd());
	}
}
