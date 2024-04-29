/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Whonames.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 09:51:39 by josorteg          #+#    #+#             */
/*   Updated: 2024/04/29 18:53:08 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Includes.hpp"


void Server::_whoServer(Client &client, std::vector<std::string> parsedCommand)
{
	//(parsedCommand[1]).erase(((parsedCommand[1]).begin()));
	if (_channelExists(parsedCommand[1]) > 0)
	{
		std::set<int> currentUsers;
		currentUsers = (getChannelbyname(parsedCommand[1])).getMembers();
		for (std::set<int>::iterator i = currentUsers.begin(); i != currentUsers.end(); ++i)
		{
			int a= *i;
			std::map<int, Client>::iterator it = _Clients.find(a);
			//std::string message = it->second.getRealname();
			std::string message = RPL_WHOREPLY(parsedCommand[1],it->second.getUsername(),it->second.getHostname(),getServername(),it->second.getNickname(),"a",it->second.getRealname()) + "\r\n";
			send(client.getFd(),message.c_str(),message.size(),0);
		}



		std::string message = RPL_ENDOFWHO(parsedCommand[1]) + "\r\n";
		send(client.getFd(),message.c_str(),message.size(),0);
	}
	else
	{
		std::string message = ERR_NOSUCHCHANNEL(parsedCommand[1]) + "\r\n";
		send(client.getFd(),message.c_str(),message.size(),0);
	}



}

//402     ERR_NOSUCHSERVER
//                        "<server name> :No such server"



// 315     RPL_ENDOFWHO
//                         "<name> :End of /WHO list"

//                 - The RPL_WHOREPLY and RPL_ENDOFWHO pair are used
//                   to answer a WHO message.  The RPL_WHOREPLY is only
//                   sent if there is an appropriate match to the WHO
//                   query.  If there is a list of parameters supplied
//                   with a WHO message, a RPL_ENDOFWHO must be sent
//                   after processing each list item with <name> being
//                   the item.

