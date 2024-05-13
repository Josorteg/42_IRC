/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeFlagI.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 19:43:31 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/13 20:05:07 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"./../inc/Includes.hpp"

void Server::_modeHandleInviteOnly (Client &client, Channel &channel, std::pair<std::string, std::string> parsedFlag)
{
	std::cout<<"Hello i am in function _modeHandleInviteOnly"<< client.getFd() << " " << channel.getName()
	 << " " << parsedFlag.first << " " << parsedFlag.second << std::endl;

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
