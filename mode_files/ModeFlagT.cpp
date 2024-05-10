/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeFlagT.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 19:44:29 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/10 19:18:00 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"./../inc/Includes.hpp"

void Server::_modeHandleTopic (Client &client, Channel &channel, std::pair<std::string, std::string> parsedFlag)
{
    std::cout<<"Hello i am in function _modeHandleTopic"<< client.getFd() << " " << channel.getName()
     << " " << parsedFlag.first << " " << parsedFlag.second << std::endl;

    if (parsedFlag.first[0] == '+')
        channel.set_t(true);
    else
        channel.set_t(false);
    _sendMessage(channel,0, RPL_CHANNELMODEIS(_getServername(),client.getNickname(),channel.getName(), parsedFlag.first));
}
