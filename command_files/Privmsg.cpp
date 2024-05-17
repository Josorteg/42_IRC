/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 17:41:36 by josorteg          #+#    #+#             */
/*   Updated: 2024/05/17 18:53:19 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"./../inc/Includes.hpp"

void Server::_privmsgServer(Client &client, std::vector<std::string> parsedCommand)
{
	/* PRIVMSG JohnDoe,Mimi :Hello, how are you? */

	std::string message;

	if (parsedCommand.size() > 1 && parsedCommand[1][0] == ':')
		return(_sendMessage(client, ERR_NORECIPIENT(_getServername())));
	if (parsedCommand.size() < 3 || parsedCommand[2][0] != ':')
		return(_sendMessage(client, ERR_NOTEXTTOSEND(_getServername())));

	std::vector<std::string> receivers = _splitString(parsedCommand[1], ',');
		
	for (size_t i = 0; i < receivers.size(); ++i)
	{
		if (receivers[i][0] == '#') // check if is a channel
			_handleMessageToChannel(client, receivers[i], parsedCommand[2]);
		else
			_handleMessageToUser(client, receivers[i], parsedCommand[2]);
	}
}

void Server::_handleMessageToUser(Client &client, std::string receiver, std::string message)
{
	int clientToSendMessageFd = _getClientfdByName(receiver);
	if (clientToSendMessageFd == 0)
		return(_sendMessage(client, ERR_NOSUCHNICK(_getServername(), receiver)));
	message = ":" + client.getNickname() + "!" + client.getHostname() + " PRIVMSG " + receiver + " " + message;
	std::map<int, Client>::iterator it = _Clients.find(clientToSendMessageFd);
	_sendMessage(it->second, message);
}

void Server::_handleMessageToChannel(Client &client, std::string receiver, std::string message)
{
	if (!_channelExists(receiver))
		return(_sendMessage(client, ERR_NOSUCHCHANNEL((receiver))));

	Channel& channel = _getChannelbyname(receiver);
	std::set<int> listOfMembers = channel.getMembers();
	message = ":" + client.getNickname() + "!" + client.getHostname() + " PRIVMSG " + receiver + " " + message;

	if (!channel.isMember(client.getFd()))
		return(_sendMessage(client, ERR_CANNOTSENDTOCHAN(_getServername(), receiver)));
	_sendMessage(channel,client.getFd(),message);
}

