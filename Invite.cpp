/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 16:57:13 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/01 17:56:17 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes.hpp"

void Server::_inviteServer(Client &client, std::vector<std::string> parsedCommand)
{
    /*INVITE JohnDoe #example-channel */
    
    std::string message;
	
	if (parsedCommand.size() != 3)
		return(_sendMessage(client, ERR_NEEDMOREPARAMS(parsedCommand[0])));
    size_t nbr = _channelExists((parsedCommand[2]));
	if (nbr)
	{
		Channel channel = getChannelbyname((parsedCommand[2]));
        int invitedClientFd = _getClientfdByName(parsedCommand[1]);

        if (invitedClientFd == 0) //check if invited client exists
	        return(_sendMessage(client, ERR_NOSUCHNICK(getServername(), parsedCommand[1])));   
		if (channel.isMember(invitedClientFd) || channel.isInvited(invitedClientFd))  //check if invited client is member of the  channel or is invited
			return(_sendMessage(client, ERR_USERONCHANNEL(getServername(), parsedCommand[1], parsedCommand[2])));
        if (!channel.isMember(client.getFd())) //check if client who is inviting is member of the channel
			return(_sendMessage(client, ERR_NOTONCHANNEL(getServername(), parsedCommand[2]))); 
		if (!channel.isOperator(client.getFd())) //check if client who is inviting is operator
			return(_sendMessage(client, ERR_CHANOPRIVSNEEDED(getServername(), parsedCommand[2])));
        
		// reply to both users:  <your_nickname> INVITE <target_nickname> :<channel>
		message = client.getNickname() + parsedCommand[0] + parsedCommand[1] + parsedCommand[2];	
        std::map<int, Client>::iterator i = _Clients.find(invitedClientFd);	
        _sendMessage(client, message);
		_sendMessage(i->second, message);
       // _sendMessage(client, RPL_INVITING(getServername(), parsedCommand[1], parsedCommand[2])); this is from protocol, maybe we should also send it, its prepared to for test
       // _sendMessage(i->second, RPL_INVITING(getServername(), parsedCommand[1], parsedCommand[2]));
	
	}
	else
		_sendMessage(client, ERR_NOSUCHCHANNEL(parsedCommand[0]));
}