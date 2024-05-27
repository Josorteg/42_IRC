/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 13:40:20 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/27 18:53:24 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"./../inc/Includes.hpp"

void Server::_topicServer(Client &client, std::vector<std::string> parsedCommand)
{
	//TOPIC #abc :New topic name
	//TOPIC #test ; check the topic for #test.

	if (parsedCommand.size() < 2 || (parsedCommand.size() > 2 && parsedCommand[2][0] != ':'))
		return(_sendMessage(client, ERR_NEEDMOREPARAMS(parsedCommand[0])));
	if (!_channelExists(parsedCommand[1]))
		return(_sendMessage(client, ERR_NOSUCHCHANNEL(parsedCommand[0])));

	Channel& channel = _getChannelbyname((parsedCommand[1]));
	std::set<int> listOfMembers = channel.getMembers();
	std::string message;

	if (!channel.isMember(client.getFd()))
		return(_sendMessage(client, ERR_NOTONCHANNEL(_getServername(), channel.getName())));
	if (parsedCommand.size() == 2)
	{
		if (channel.getTopic().empty())
			return(_sendMessage(client, RPL_NOTOPIC(_getServername(),channel.getName())));
		else
			return(_sendMessage(client, RPL_TOPIC(_getServername(),channel.getName(),channel.getTopic())));
	}
	if (channel.get_t() == true && !channel.isOperator(client.getFd()))
		return(_sendMessage(client, ERR_CHANOPRIVSNEEDED(_getServername(), channel.getName())));
		
	channel.setTopic(parsedCommand[2]);
	message = ":" + client.getNickname() + "!" + client.getHostname() + " TOPIC " + channel.getName();
	if (!channel.getTopic().empty())
		message += " " + channel.getTopic();
	_sendMessage(channel, 0, message);
}
