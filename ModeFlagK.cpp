/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeFlagK.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 19:44:04 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/05 16:09:56 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes.hpp"

void Server::_modeHandlePassword (Client &client, Channel &channel, std::pair<std::string, std::string> parsedFlag)
{
    std::cout<<"Hello i am in function _modeHandlePassword"<< client.getFd() << " " << channel.getName()
     << " " << parsedFlag.first << " " << parsedFlag.second << std::endl;

    std::string command = "MODE"; //I will change this

    if (parsedFlag.first[0] == '+')
    {
        if (parsedFlag.second == channel.getPassword())
            return(_sendMessage(client, ERR_KEYSET(getServername(),command)));

        channel.set_k(true);
        channel.setPassword(parsedFlag.second);
    }
    else
    {
        //if (parsedFlag.second != "*" && parsedFlag.second != channel.getPassword()) -i dont think we need this
        channel.set_k(false);
        channel.setPassword("");
    }
    if (parsedFlag.second != "empty")
        parsedFlag.first.append(" ").append(parsedFlag.second);
    _sendMessage(channel,0, RPL_CHANNELMODEIS(getServername(),client.getNickname(),channel.getName(), parsedFlag.first));
}
