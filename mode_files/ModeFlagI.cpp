/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeFlagI.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 19:43:31 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/27 18:54:11 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"./../inc/Includes.hpp"

void Server::_modeHandleInviteOnly (Client &client, Channel &channel, std::pair<std::string, std::string> parsedFlag)
{
	if (parsedFlag.first[0] == '+')
	{
		channel.set_i(true);
		std::cout<<"CHANNEL: "<<channel.getName()<<"set as ONLY INVITED MODE!!!!"<<"BOOLEAN: "<<channel.get_i()<<std::endl;
	}
	else
	{
		channel.set_i(false);
		std::cout<<"CHANNEL: "<<channel.getName()<<"UNset as ONLY INVITED MODE!!!!"<<"BOOLEAN: "<<channel.get_i()<<std::endl;
	}
	_sendMessage(channel,0, RPL_CHANNELMODEIS(_getServername(),client.getNickname(),channel.getName(), parsedFlag.first));
}
