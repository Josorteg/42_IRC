/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeFlagK.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 19:44:04 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/03 21:44:47 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes.hpp"

void Server::_modeHandlePassword (Client &client, Channel &channel, std::pair<std::string, std::string> parsedFlag)
{
    std::cout<<"Hello i am in function _modeHandlePassword"<< client.getFd() << " " << channel.getName()
     << " " << parsedFlag.first << " " << parsedFlag.second << std::endl;

    std::string command = "MODE"; //I will change this

    if (parsedFlag.second == "empty")
        return(_sendMessage(client, ERR_NEEDMOREPARAMS(command)));
    
    if (parsedFlag.first[0] == '+')
    {
        if (parsedFlag.second == channel.getPassword())
            return(_sendMessage(client, ERR_KEYSET(command)));
        
        channel.set_k(true);
        channel.setPassword(parsedFlag.second);
        parsedFlag.first.append(" ").append(parsedFlag.second); //from <+k, 123> i make <+k 123,123> for send in RPL_CHANNELMODEIS
    }
    else
    {
        //if (parsedFlag.second != "*" && parsedFlag.second != channel.getPassword()) -i dont think we need this    
        channel.set_k(false);
        channel.setPassword("");
    }

    std::set<int> currentUsers;
	currentUsers = channel.getMembers();
	for (std::set<int>::iterator i = currentUsers.begin(); i != currentUsers.end(); ++i)
	{
		int a= *i;
		std::map<int, Client>::iterator it = _Clients.find(a);
        _sendMessage(it->second, RPL_CHANNELMODEIS(getServername(),client.getNickname(),channel.getName(), parsedFlag.first));
	}
}