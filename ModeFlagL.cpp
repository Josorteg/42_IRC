/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeFlagL.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 19:44:43 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/03 22:02:18 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes.hpp"

void Server::_modeHandleLimit (Client &client, Channel &channel, std::pair<std::string, std::string> parsedFlag)
{
    std::cout<<"Hello i am in function _modeHandleLimit"<< client.getFd() << " " << channel.getName()
     << " " << parsedFlag.first << " " << parsedFlag.second << std::endl;

    std::string command = "MODE"; //I will change this

    if (parsedFlag.second == "empty")
        return(_sendMessage(client, ERR_NEEDMOREPARAMS(command)));
    
    if (parsedFlag.first[0] == '+')
    {
  	    int strlen = parsedFlag.second.length();
        
        for (int i = 0; i < strlen; i++)
	    {
		    if (i == 0 && (parsedFlag.second[0] == '+'))
			    continue;
		    else if (i > 0 && !std::isdigit(parsedFlag.second[i]))
			{
                return(_sendMessage(client, ERR_NEEDMOREPARAMS(command)));
            }
	    }

		long limitValue;
		std::string posString = parsedFlag.second;
		std::istringstream iss(posString);
		if (!(iss >> limitValue) || (limitValue < INT_MIN || limitValue > INT_MAX))
			return(_sendMessage(client, ERR_NEEDMOREPARAMS(command)));      
        channel.set_l(true);
        channel.setClientLimit(static_cast<int>(limitValue));
        parsedFlag.first.append(" ").append(parsedFlag.second); //from <+i, 123> i make <+i 123,123> for send in RPL_CHANNELMODEIS
    }
    else
    {  
        channel.set_l(false);
        channel.setClientLimit(0);
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