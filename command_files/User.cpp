/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 11:38:52 by josorteg          #+#    #+#             */
/*   Updated: 2024/05/30 00:19:08 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../inc/Includes.hpp"

//USER 			josorteg 	0 			* 			Jose Miguel Ortega Sanchez
//Parameters: 	<username> <hostname> <servername> <realname>

bool Server::_userServer(Client &client, std::vector<std::string> parsedCommand)
{
	if (client.getIsRegistered())
	{
		_sendMessage(client, ERR_ALREADYREGISTRED());
		return(false);
	}
	if (parsedCommand.size() < 5)
	{
		_sendMessage(client, ERR_NEEDMOREPARAMS(parsedCommand[0]));
		return(false);
	}
	client.setUsername(parsedCommand[1]);
	client.setRealname(parsedCommand[4]);
	client.setHostname(parsedCommand[1] + "@127.0.0.1");

	if (!client.getNickname().empty() && client.getHasPassword())
	{
		client.setIsRegistered(true);

		_sendMessage(client, RPL_WELCOME(client.getNickname(), this->_getServername() ,client.getHostname()));
		_sendMessage(client, RPL_YOURHOST(this->_getServername(),client.getNickname()));
		_sendMessage(client, RPL_CREATED(this->_getServername(),this->_getTime()));
		_sendMessage(client, RPL_MYINFO(this->_getServername(),client.getNickname()));
	}
	return(true);
}
