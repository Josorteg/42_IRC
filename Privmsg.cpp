/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 17:41:36 by josorteg          #+#    #+#             */
/*   Updated: 2024/05/01 15:56:23 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Includes.hpp"


void Server::_privmsgServer(Client &client, std::vector<std::string> parsedCommand)
{
	std::string message;

	if (parsedCommand.size() > 1 && parsedCommand[1][0] == ':')
		return(_sendMessage(client, ERR_NORECIPIENT(getServername())));
	if (parsedCommand.size() == 2)
		return(_sendMessage(client, ERR_NOTEXTTOSEND(getServername())));	
	
	std::vector<std::string> receivers = _splitString(parsedCommand[1], ',');
	int fd; //delete

	std::string text ="";
	for (size_t j = 2;j < parsedCommand.size();j++)
	{
		text += " " + parsedCommand[j];
	}
/*
PRIVMSG JohnDoe,Mimi :Hello, how are you?
*/
fd = client.getFd(); //delete
//check errors

	for (size_t i = 0; i < receivers.size(); ++i)
	{
		if (receivers[i][0] == '#') // check if is a channel
		{ //channel message
			;
		}
		else //person to person message
		{
			//check if person exists
			int clientToSendMessageFd = _getClientfdByName(receivers[i]);
			if (clientToSendMessageFd == 0)
			{
				_sendMessage(client, ERR_NOSUCHNICK(getServername(), receivers[i]));
			}
			else
			{
				message = ":" + client.getNickname() + "!" + client.getHostname() + " "
					+ parsedCommand[0] + " " + receivers[i] + text;
				std::map<int, Client>::iterator it = _Clients.find(clientToSendMessageFd);
				_sendMessage(it->second, message);
				std::cout<<"_privmsgServer: message is " << message << std::endl;
			}
		}
	}
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
		{
			listOfClients += parsedCommand[i] + " ";	
		}	
	}					
	std::cout<<"_isonServer: message is " << message << std::endl;
	_sendMessage(client, RPL_ISON(getServername(), client.getNickname(),listOfClients));
}
