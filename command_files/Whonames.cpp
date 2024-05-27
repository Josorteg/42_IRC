/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Whonames.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 09:51:39 by josorteg          #+#    #+#             */
/*   Updated: 2024/05/27 18:53:06 by mmoramov         ###   ########.fr       */
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