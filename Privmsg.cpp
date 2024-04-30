/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 17:41:36 by josorteg          #+#    #+#             */
/*   Updated: 2024/04/30 18:41:57 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Includes.hpp"


void Server::_privmsgServer(Client &client, std::vector<std::string> parsedCommand)
{
	std::string message;
	int fd;

	std::string text ="";
	for (size_t j = 2 ; j < parsedCommand.size(); j++)
	{
		text = text + " " + parsedCommand[j];
	}
/*
PRIVMSG JohnDoe :Hello, how are you?
*/
fd = client.getFd();
//check errors

// check if is a channel

if (parsedCommand[1][0] == '#')
{ //channel message
	;
}
else //person to person message
{
	//check if person exists
	int clientToSendMessageFd = _getClientfdByName(parsedCommand[1]);
	if (clientToSendMessageFd == 0)
	{
		//didnt find a client
		;
	}
	else
	{
		//i found a client


		message = ":" + client.getNickname() + "!" + client.getHostname() + " "
			+ parsedCommand[0] + " " + parsedCommand[1] + text
		+ "\r\n";
		send(clientToSendMessageFd,message.c_str(),message.size(),0);
	}

}
}

void Server::_isonServer(Client &client, std::vector<std::string> parsedCommand)
{
	int clientToSendMessageFd = _getClientfdByName(parsedCommand[1]);
	if (clientToSendMessageFd == 0)
	{
		std::string message =  (":" + getServername() + " 303 "+ client.getNickname() + " \r\n");
		send(client.getFd(),message.c_str(),message.size(),0);
	}
	else
	{
		std::string message =  (":" + getServername() + " 303 "+ client.getNickname() + " :" + parsedCommand[1]+" \r\n");
		send(client.getFd(),message.c_str(),message.size(),0);
	}

}
