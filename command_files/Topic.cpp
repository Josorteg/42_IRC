/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 13:40:20 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/10 12:12:38 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"./../inc/Includes.hpp"

void Server::_topicServer(Client &client, std::vector<std::string> parsedCommand)
{
	std::cout<<"Hello i am in function _topicServer "<< client.getFd() << " " << parsedCommand[0] << std::endl;

	//TOPIC #abc :New topic name
	//TOPIC #test ; check the topic for #test.

	if (parsedCommand.size() < 2)
		return(_sendMessage(client, ERR_NEEDMOREPARAMS(parsedCommand[0])));
	if (!_channelExists(parsedCommand[1]))
		return(_sendMessage(client, ERR_NOSUCHCHANNEL(parsedCommand[0])));

	Channel& channel = _getChannelbyname((parsedCommand[1]));
	std::set<int> listOfMembers = channel.getMembers();
	std::string message;
	std::string topic = "";
	for (size_t j = 2;j < parsedCommand.size();j++)
		topic += " " + parsedCommand[j];

	if (!channel.isMember(client.getFd()))
		return(_sendMessage(client, ERR_NOTONCHANNEL(getServername(), channel.getName())));
	if (parsedCommand.size() == 2)
	{
		if (channel.getTopic().empty())
			return(_sendMessage(client, RPL_NOTOPIC(getServername(),channel.getName())));
		else
			return(_sendMessage(client, RPL_TOPIC(getServername(),channel.getName(),channel.getTopic())));
	}
	if (channel.get_t() == true && !channel.isOperator(client.getFd()))
		return(_sendMessage(client, ERR_CHANOPRIVSNEEDED(getServername(), channel.getName())));

	channel.setTopic(topic);

	message = ":" + client.getNickname() + "!" + client.getHostname() + " TOPIC " + channel.getName();
	if (!channel.getTopic().empty())
		message += channel.getTopic();
	_sendMessage(channel, 0, message);
}
