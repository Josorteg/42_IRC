/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeFlagO.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 19:43:48 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/05 15:54:42 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes.hpp"

void Server::_modeHandleOperatorPrivileges (Client &client, Channel &channel, std::pair<std::string, std::string> parsedFlag)
{
    std::cout<<"Hello i am in function _modeHandleOperatorPrivileges"<< client.getFd() << " " << channel.getName()
     << " " << parsedFlag.first << " " << parsedFlag.second << std::endl;

    std::string command = "MODE"; //I will change this
    int NewOperatorFd = _getClientfdByName(parsedFlag.second);

    if (NewOperatorFd == 0) //check if new operator exists
	     return(_sendMessage(client, ERR_NOSUCHNICK(getServername(), parsedFlag.second)));
    if (!channel.isMember(NewOperatorFd))  //check if new operator is member of the channel
		return(_sendMessage(client, ERR_NOTONCHANNEL(getServername(), channel.getName())));
    std::map<int, Client>::iterator it = _Clients.find(NewOperatorFd);


    if (parsedFlag.first[0] == '+')
       channel.addOperator(it->second);
    else
        channel.removeOperator(it->second);
    parsedFlag.first.append(" ").append(parsedFlag.second);
	_sendMessage(channel,0, RPL_CHANNELMODEIS(getServername(),client.getNickname(),channel.getName(), parsedFlag.first));

}
