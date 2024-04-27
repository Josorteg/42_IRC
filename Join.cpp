/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 14:54:15 by mmoramov          #+#    #+#             */
/*   Updated: 2024/04/27 16:49:33 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Includes.hpp"


void Server::_joinServer(Client &client, std::vector<std::string> parsedCommand)
{
   
	//check if username has 2 parameters
	if (parsedCommand.size() < 2)
	{
		std::string message = ERR_NEEDMOREPARAMS(parsedCommand[0]) + "\r\n";
		send(client.getFd(),message.c_str(),message.size(),0);
		_rmClient(client);
		return;
	}
	
	

    if (parsedCommand.size() == 2)
	{
		// JOIN #foobar 
		Channel newChannel(this, parsedCommand[1], client);

		if (!newChannel._nameCheck(parsedCommand[1]))
		{
			std::string message = ERR_NOSUCHCHANNEL(parsedCommand[1]) + "\r\n";
			send(client.getFd(),message.c_str(),message.size(),0);
			_rmClient(client);
			return;
		}
		//parsedCommand.erase(parsedCommand.begin());
		//i check if canal exists
		std::cout<<"lets check if channel exists: " << parsedCommand[1] <<std::endl;
		size_t i = _channelExists(parsedCommand[1]);
		std::cout<<"found number: " << i <<std::endl;
		if (i)
		{
			std::cout<<"Yes channel exists: " << parsedCommand[1] <<std::endl;
			_Channels[i - 1].addClient(client);
			std::string message = "Added to the channel\r\n";
			send(client.getFd(),message.c_str(),message.size(),0);  
		}
		else
		{
			std::cout<<"No channel doesnt exists: " << parsedCommand[1] <<std::endl;
			std::cout<<"Size before pushback: " << _Channels.size() <<std::endl;
			_Channels.push_back(newChannel);
			std::cout<<"Size after pushback: " << _Channels.size() <<std::endl;
			std::string message = "create a new channel\r\n";
			send(client.getFd(),message.c_str(),message.size(),0);  
		}
		
	}

    



 // if JOIN is successful, the user is then sent the channel's topic
//    (using RPL_TOPIC) and the list of users who are on the channel (using
//    RPL_NAMREPLY), which must include the user joining.

    
}

 