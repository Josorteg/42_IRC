/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 15:15:44 by josorteg          #+#    #+#             */
/*   Updated: 2024/05/01 15:50:52 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Includes.hpp"

void Server::_modeServer(Client &client, std::vector<std::string> parsedCommand)
{
	if (parsedCommand[2].empty() || parsedCommand[2] == "b")
	{
		_sendMessage(client, RPL_CHANNELMODEIS(getServername(),client.getNickname(),(parsedCommand[1])));
		_sendMessage(client, RPL_ENDOFBANLIST((parsedCommand[1])));
	}
}
