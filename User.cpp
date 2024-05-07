/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 11:38:52 by josorteg          #+#    #+#             */
/*   Updated: 2024/05/07 20:31:31 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Includes.hpp"

//USER 			josorteg 	0 			* 			Jose Miguel Ortega Sanchez
//Parameters: 	<username> <hostname> <servername> <realname>

void Server::_userServer(Client &client, std::vector<std::string> parsedCommand)
{
	if (parsedCommand.size() < 5)
	{
		_sendMessage(client, ERR_NEEDMOREPARAMS(parsedCommand[0]));
		_rmClient(client);
		return;
	}
	if (client.getIsRegistered())
	{
		_sendMessage(client, ERR_ALREADYREGISTRED());
		_rmClient(client);
		return;
	}
	client.setUsername(parsedCommand[1]);
	client.setRealname(parsedCommand[4]);
	client.setHostname(parsedCommand[1] + "@127.0.0.1");

	if (!client.getNickname().empty() && client.getHasPassword())
		client.setIsRegistered(true);

	_sendMessage(client, RPL_WELCOME(client.getNickname(), this->getServername() ,client.getHostname()));
	_sendMessage(client, RPL_YOURHOST(this->getServername(),client.getNickname()));
	_sendMessage(client, RPL_CREATED(this->getServername(),this->getTime()));
	_sendMessage(client, RPL_MYINFO(this->getServername(),client.getNickname()));
}
