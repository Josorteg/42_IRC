/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeFlagK.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 19:44:04 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/13 19:42:47 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"./../inc/Includes.hpp"

void Server::_modeHandlePassword (Client &client, Channel &channel, std::pair<std::string, std::string> parsedFlag)
{
    std::cout<<"Hello i am in function _modeHandlePassword"<< client.getFd() << " " << channel.getName()
     << " " << parsedFlag.first << " " << parsedFlag.second << std::endl;

    if (parsedFlag.first[0] == '+')
    {
        if (parsedFlag.second == channel._getPassword())
            return(_sendMessage(client, ERR_KEYSET(_getServername(),channel.getName())));

        channel.set_k(true);
        channel.setPassword(parsedFlag.second);
    }
    else
    {
        //if (parsedFlag.second != "*" && parsedFlag.second != channel._getPassword()) -i dont think we need this
        channel.set_k(false);
        channel.setPassword("");
    }
    if (parsedFlag.second != "empty")
        parsedFlag.first.append(" ").append(parsedFlag.second);
    _sendMessage(channel,0, RPL_CHANNELMODEIS(_getServername(),client.getNickname(),channel.getName(), parsedFlag.first));
}
