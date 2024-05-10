/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeFlagL.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 19:44:43 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/10 12:12:38 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"./../inc/Includes.hpp"

void Server::_modeHandleLimit (Client &client, Channel &channel, std::pair<std::string, std::string> parsedFlag)
{
    std::cout<<"Hello i am in function _modeHandleLimit"<< client.getFd() << " " << channel.getName()
     << " " << parsedFlag.first << " " << parsedFlag.second << std::endl;

    std::string command = "MODE"; //I will change this

    if (parsedFlag.first[0] == '+')
    {
  	    int strlen = parsedFlag.second.length();

        for (int i = 0; i < strlen; i++)
	    {
		    if (i == 0 && (parsedFlag.second[0] == '+'))
			    continue;
		    else if (i > 0 && !std::isdigit(parsedFlag.second[i]))
			{
                return(_sendMessage(client, ERR_NEEDMOREPARAMS(command))); //?
            }
	    }

		long limitValue;
		std::string posString = parsedFlag.second;
		std::istringstream iss(posString);
		if (!(iss >> limitValue) || limitValue > INT_MAX) //int max?
			return(_sendMessage(client, ERR_NEEDMOREPARAMS(command)));      //?
        channel.set_l(true);
        channel.setClientLimit(static_cast<int>(limitValue));
        parsedFlag.first.append(" ").append(parsedFlag.second); //from <+i, 123> i make <+i 123,123> for send in RPL_CHANNELMODEIS
    }
    else
    {
        channel.set_l(false);
        channel.setClientLimit(0);//??
    }
    _sendMessage(channel,0, RPL_CHANNELMODEIS(getServername(),client.getNickname(),channel.getName(), parsedFlag.first));
}
