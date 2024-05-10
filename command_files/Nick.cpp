/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 10:04:35 by josorteg          #+#    #+#             */
/*   Updated: 2024/05/10 11:28:18 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"./../inc/Includes.hpp"

static bool	nickChecker(std::string nick)
{
	std::size_t found = nick.find_first_not_of(NICK_CHARACTERS);
	if (found != std::string::npos)
		return true;
	return false;
}
void Server::_nickServer(Client &client, std::vector<std::string> parsedCommand)
{
	std::string message;

	if (parsedCommand.size() < 2)
	{
		_sendMessage(client, ERR_NONICKNAMEGIVEN());
		_rmClient(client);
		return;
	}
	if (nickChecker(parsedCommand[1]))
	{
		_sendMessage(client, ERR_ERRONEUSNICKNAME(parsedCommand[1]));
		_rmClient(client);
		return;
	}
	if (_getClientfdByName(parsedCommand[1]) != 0)
	{
		_sendMessage(client, ERR_NICKNAMEINUSE(parsedCommand[1]));
		_rmClient(client);
		return;
	}
	client.setNickname(parsedCommand[1]);
	if (client.getIsRegistered())
	{
		message = "Nick was changed to : " + client.getNickname();
		_sendMessage(client, message);
	}
	if (!client.getUsername().empty() && client.getHasPassword())
		client.setIsRegistered(true);
}
