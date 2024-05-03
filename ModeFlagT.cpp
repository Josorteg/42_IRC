/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeFlagT.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 19:44:29 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/03 21:14:31 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes.hpp"

void Server::_modeHandleTopic (Client &client, Channel &channel, std::pair<std::string, std::string> parsedFlag)
{
    std::cout<<"Hello i am in function _modeHandleTopic"<< client.getFd() << " " << channel.getName()
     << " " << parsedFlag.first << " " << parsedFlag.second << std::endl;

    if (parsedFlag.first[0] == '+')
        channel.set_t(true);
    else
        channel.set_t(false); 

    std::set<int> currentUsers;
	currentUsers = channel.getMembers();
	for (std::set<int>::iterator i = currentUsers.begin(); i != currentUsers.end(); ++i)
	{
		int a= *i;
		std::map<int, Client>::iterator it = _Clients.find(a);
        _sendMessage(it->second, RPL_CHANNELMODEIS(getServername(),client.getNickname(),channel.getName(), parsedFlag.first));
	}
}

/*  by mistake i handled TOPIC command, i will leave it to future use 
{

        channel.setTopic(parsedFlag.second);  // i think now it works if topic is one word
    }
    else

        channel.setTopic(""); 
    }
    _sendMessage(client, RPL_CHANNELMODEIS(getServername(),client.getNickname(),channel.getName(), parsedFlag.first));
    _sendMessage(client, RPL_TOPIC(getServername(),channel.getName(),channel.getTopic()));
    */