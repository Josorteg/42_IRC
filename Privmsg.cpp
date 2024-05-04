/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 17:41:36 by josorteg          #+#    #+#             */
/*   Updated: 2024/05/04 16:26:35 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Includes.hpp"

void Server::_privmsgServer(Client &client, std::vector<std::string> parsedCommand)
{
	/* PRIVMSG JohnDoe,Mimi :Hello, how are you? */

	std::string message;
	std::string text = "";

	if (parsedCommand.size() > 1 && parsedCommand[1][0] == ':')
		return(_sendMessage(client, ERR_NORECIPIENT(getServername())));
	if (parsedCommand.size() < 3)
		return(_sendMessage(client, ERR_NOTEXTTOSEND(getServername())));

	std::vector<std::string> receivers = _splitString(parsedCommand[1], ',');
	for (size_t j = 2;j < parsedCommand.size();j++)
		text += " " + parsedCommand[j];
	for (size_t i = 0; i < receivers.size(); ++i)
	{
		if (receivers[i][0] == '#') // check if is a channel
			_handleMessageToChannel(client, receivers[i], text);
		else
			_handleMessageToUser(client, receivers[i], text);
	}
}

void Server::_handleMessageToUser(Client &client, std::string receiver, std::string message)
{
	int clientToSendMessageFd = _getClientfdByName(receiver);
	if (clientToSendMessageFd == 0)
		return(_sendMessage(client, ERR_NOSUCHNICK(getServername(), receiver)));
	message = ":" + client.getNickname() + "!" + client.getHostname() + " PRIVMSG " + receiver + message;
	std::map<int, Client>::iterator it = _Clients.find(clientToSendMessageFd);
	_sendMessage(it->second, message);
}

void Server::_handleMessageToChannel(Client &client, std::string receiver, std::string message)
{
	if (!_channelExists(receiver))
		return(_sendMessage(client, ERR_NOSUCHCHANNEL((receiver))));

	Channel& channel = getChannelbyname(receiver);
	std::set<int> listOfMembers = channel.getMembers();
	message = ":" + client.getNickname() + "!" + client.getHostname() + " PRIVMSG " + receiver + message;

	if (!channel.isMember(client.getFd()))
		return(_sendMessage(client, ERR_CANNOTSENDTOCHAN(getServername(), receiver)));
	_sendMessage(channel,client.getFd(),message);
}

void Server::_isonServer(Client &client, std::vector<std::string> parsedCommand)
{
	std::string message;
	std::string listOfClients = "";

	if (parsedCommand.size() < 2)
		return(_sendMessage(client, ERR_NEEDMOREPARAMS(parsedCommand[0])));
	for (size_t i = 1; i < parsedCommand.size(); ++i)
	{
		if (_getClientfdByName(parsedCommand[i]) != 0)
			listOfClients += parsedCommand[i] + " ";
	}
	std::cout<<"_isonServer: message is " << message << std::endl;
	_sendMessage(client, RPL_ISON(getServername(), client.getNickname(),listOfClients));
}
