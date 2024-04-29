/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 15:15:44 by josorteg          #+#    #+#             */
/*   Updated: 2024/04/29 16:26:51 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Includes.hpp"

void Server::_modeServer(Client &client, std::vector<std::string> parsedCommand)
{
	//# define RPL_ENDOFBANLIST(channel)(std::string("368 " + channel + " :Fin de la lista de bans del canal"))
	if (parsedCommand[2].empty() || parsedCommand[2] == "b")
	{
		//# define RPL_CHANNELMODEIS(channel,modo,parameter)(std::string("324 " + channel + " " + modo + " " + parameter))
		std::string empty = "o";
		std::string message = RPL_CHANNELMODEIS((parsedCommand[1]),"+ops",empty) + "\r\n";
		send(client.getFd(),message.c_str(),message.size(),0);
		message = RPL_ENDOFBANLIST((parsedCommand[1])) + "\r\n";
		send(client.getFd(),message.c_str(),message.size(),0);

	}
}
