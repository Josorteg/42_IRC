/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 13:40:20 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/07 19:12:45 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes.hpp"

void Server::_topicServer(Client &client, std::vector<std::string> parsedCommand)
{
	std::cout<<"Hello i am in function _topicServer "<< client.getFd() << " " << parsedCommand[0] << std::endl;
	std::string message;
	//TOPIC #abc :New topic name
	//TOPIC #test ; check the topic for #test.

	if (parsedCommand.size() < 2)
		return(_sendMessage(client, ERR_NEEDMOREPARAMS(parsedCommand[0])));
	if (!_channelExists(parsedCommand[1]))
		return(_sendMessage(client, ERR_NOSUCHCHANNEL(parsedCommand[0])));

	Channel& channel = _getChannelbyname((parsedCommand[1]));
	std::set<int> listOfMembers = channel.getMembers();
	std::string topic = "";
	for (size_t j = 2;j < parsedCommand.size();j++)
		topic += " " + parsedCommand[j];
	std::cout << "TOPIC is"<< topic << std::endl;
	if (!channel.isMember(client.getFd()))
		return(_sendMessage(client, ERR_NOTONCHANNEL(getServername(), channel.getName())));
	if (parsedCommand.size() == 2)
	{
		if (channel.getTopic().empty())
			return(_sendMessage(client, RPL_NOTOPIC(getServername(),channel.getName())));
		else
			return(_sendMessage(client, RPL_TOPIC(getServername(),channel.getName(),channel.getTopic())));
	}
	std::cout << "TOPIC I have more params"<< std::endl;
	if (channel.get_t() == true && !channel.isOperator(client.getFd()))
		return(_sendMessage(client, ERR_CHANOPRIVSNEEDED(getServername(), channel.getName())));
	std::cout << "TOPIC before "<< channel.getTopic() << std::endl;
	channel.setTopic(topic);
	std::cout << "TOPIC after "<< channel.getTopic() << std::endl;


	//message = ":" + client.getNickname() + "!" + client.getHostname() + " TOPIC " + channel.getName() + channel.getTopic();

	if (channel.getTopic().empty())
		message = ":" + client.getNickname() + "!" + client.getHostname() + " TOPIC " + channel.getName();
		//return(_sendMessage(channel,0, RPL_NOTOPIC(getServername(),channel.getName())));

	else
		message = ":" + client.getNickname() + "!" + client.getHostname() + " TOPIC " + channel.getName() + channel.getTopic();
		//return(_sendMessage(channel,0, RPL_TOPIC(getServername(),channel.getName(),channel.getTopic())));
	return(_sendMessage(channel,0, message));
}
