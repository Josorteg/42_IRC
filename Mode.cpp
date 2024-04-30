/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 15:15:44 by josorteg          #+#    #+#             */
/*   Updated: 2024/04/30 12:10:47 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Includes.hpp"

void Server::_modeServer(Client &client, std::vector<std::string> parsedCommand)
{
	//# define RPL_ENDOFBANLIST(channel)(std::string("368 " + channel + " :Fin de la lista de bans del canal"))
	if (parsedCommand[2].empty() || parsedCommand[2] == "b")
	{
		//# define RRPL_CHANNELMODEIS(servername,nickname,channel)(std::string(":" + servername + " " + " 324 " + nickname + " " + channel + "+ovimptklb"))

		std::string message = RPL_CHANNELMODEIS(getServername(),client.getNickname(),(parsedCommand[1])) + "\r\n";
		send(client.getFd(),message.c_str(),message.size(),0);
		message = RPL_ENDOFBANLIST((parsedCommand[1])) + "\r\n";
		send(client.getFd(),message.c_str(),message.size(),0);

	}
}
