/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 15:15:44 by josorteg          #+#    #+#             */
/*   Updated: 2024/05/02 18:41:32 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Includes.hpp"

void Server::_modeServer(Client &client, std::vector<std::string> parsedCommand)
{
	// if (parsedCommand[2].empty() || parsedCommand[2] == "b")
	// {
	// 	_sendMessage(client, RPL_CHANNELMODEIS(getServername(),client.getNickname(),(parsedCommand[1])));
	// 	_sendMessage(client, RPL_ENDOFBANLIST((parsedCommand[1])));
	// }

	//MODE #channel +o-v username1 username2

	//check channel
	if (!_channelExists((parsedCommand[1])))
		_sendMessage(client, ERR_NOSUCHCHANNEL(parsedCommand[1]));
	std::string sign = "+-";
	std::string options = "okitl";
	for (size_t it = 0; it < parsedCommand[2].size(); it++)
	{
		if (strchr(sign,parsedCommand[2][it]) ||
	}
}
