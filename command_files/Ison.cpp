/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ison.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 18:53:24 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/30 00:05:31 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../inc/Includes.hpp"

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
	_sendMessage(client, RPL_ISON(_getServername(), client.getNickname(),listOfClients));
}
