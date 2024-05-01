/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 17:41:36 by josorteg          #+#    #+#             */
/*   Updated: 2024/05/01 16:53:08 by mmoramov         ###   ########.fr       */
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
		_sendMessage(client, ERR_NOSUCHNICK(getServername(), receiver));
	else
	{
		message = ":" + client.getNickname() + "!" + client.getHostname() + " PRIVMSG " + receiver + message;
		std::map<int, Client>::iterator it = _Clients.find(clientToSendMessageFd);
		_sendMessage(it->second, message);
	}
}

void Server::_handleMessageToChannel(Client &client, std::string receiver, std::string message)
{
	size_t nbr = _channelExists((receiver));
	if (nbr)
	{
		std::set<int> listOfMembers = getChannelbyname(receiver).getMembers();
		message = ":" + client.getNickname() + "!" + client.getHostname() + " PRIVMSG " + receiver + message;		
		
		std::set<int>::iterator it = listOfMembers.find(client.getFd()); //check if client is in channel
		if (it == listOfMembers.end())
			_sendMessage(client, ERR_CANNOTSENDTOCHAN(getServername(), receiver));
		else //if sender is in channel
		{
			for (std::set<int>::iterator it = listOfMembers.begin(); it != listOfMembers.end(); ++it)
			{
				int memberFd= *it;
				if (memberFd == client.getFd())
					break; //i dont send to myself because then i see the message in my channel twice. 
				std::map<int, Client>::iterator i = _Clients.find(memberFd);
				_sendMessage(i->second, message);
			}	
		}				
	}
	else
		_sendMessage(client, ERR_NOSUCHCHANNEL((receiver)));
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
