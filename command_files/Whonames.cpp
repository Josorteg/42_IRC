/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Whonames.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 09:51:39 by josorteg          #+#    #+#             */
/*   Updated: 2024/05/23 19:11:55 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"./../inc/Includes.hpp"


void Server::_whoServer(Client &client, std::vector<std::string> parsedCommand)
{
	//(parsedCommand[1]).erase(((parsedCommand[1]).begin()));
	if (!_channelExists(parsedCommand[1]))
		return(_sendMessage(client, ERR_NOSUCHCHANNEL(parsedCommand[1])));
	
	Channel& channel = _getChannelbyname((parsedCommand[1]));
	std::set<int> currentUsers = channel.getMembers();
	for (std::set<int>::iterator i = currentUsers.begin(); i != currentUsers.end(); ++i)
	{
		int a= *i;
		std::map<int, Client>::iterator it = _Clients.find(a);
		_sendMessage(client, RPL_WHOREPLY(parsedCommand[1],client.getNickname(),it->second.getHostname(),_getServername(),it->second.getNickname(),it->second.getRealname()));
	}
	if (channel.getBotActive() == true)
	{
		_sendMessage(client, RPL_WHOREPLY(parsedCommand[1],":BOT","BOT",_getServername(),":BOT","BOT"));
	}

	_sendMessage(client, RPL_ENDOFWHO(parsedCommand[1],_getServername(),client.getNickname()));

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

