/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeFlagI.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 19:43:31 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/30 00:20:19 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../inc/Includes.hpp"

void Server::_modeHandleInviteOnly (Client &client, Channel &channel, std::pair<std::string, std::string> parsedFlag)
{
	if (parsedFlag.first[0] == '+')
		channel.set_i(true);
	else
		channel.set_i(false);
	_sendMessage(channel,0, RPL_CHANNELMODEIS(_getServername(),client.getNickname(),channel.getName(), parsedFlag.first));
}
