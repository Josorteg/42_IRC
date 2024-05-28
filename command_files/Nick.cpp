/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 10:04:35 by josorteg          #+#    #+#             */
/*   Updated: 2024/05/28 18:13:22 by josorteg         ###   ########.fr       */
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
bool Server::_nickServer(Client &client, std::vector<std::string> parsedCommand)
{
	std::string message;
	std::string oldNickname;

	if (parsedCommand.size() < 2)
	{
		_sendMessage(client, ERR_NONICKNAMEGIVEN());
		return(false);
	}
	if (nickChecker(parsedCommand[1]))
	{
		_sendMessage(client, ERR_ERRONEUSNICKNAME(parsedCommand[1]));
		return(false);
	}
	if (_getClientfdByName(parsedCommand[1]) != 0)
	{
		_sendMessage(client, ERR_NICKNAMEINUSE(parsedCommand[1]));
		return(false);
	}

	if (client.getIsRegistered())
	{
		oldNickname = client.getNickname();
		client.setNickname(parsedCommand[1]);

		message = "Nick was changed to : " + client.getNickname();
		_sendMessage(client, message);

		//:old_nickname!user@host NICK :new_nickname

		message = ":" + oldNickname + "!" + client.getHostname() + " NICK :" + client.getNickname();
		//_sendMessage(client, message);

		for (std::map<int, Client>::iterator it = _Clients.begin(); it != _Clients.end(); ++it)
   		{
			_sendMessage(it->second, message);
		}
	}
	else if (!client.getUsername().empty() && client.getHasPassword())
	{
		client.setNickname(parsedCommand[1]);
		client.setIsRegistered(true);

		_sendMessage(client, RPL_WELCOME(client.getNickname(), this->_getServername() ,client.getHostname()));
		_sendMessage(client, RPL_YOURHOST(this->_getServername(),client.getNickname()));
		_sendMessage(client, RPL_CREATED(this->_getServername(),this->_getTime()));
		_sendMessage(client, RPL_MYINFO(this->_getServername(),client.getNickname()));
	}
	return(true);
}
