/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 16:57:13 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/04 17:41:18 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes.hpp"

void Server::_inviteServer(Client &client, std::vector<std::string> parsedCommand)
{
    /*INVITE JohnDoe #example-channel */

    std::string message;

	if (parsedCommand.size() != 3)
		return(_sendMessage(client, ERR_NEEDMOREPARAMS(parsedCommand[0])));
	if (!_channelExists(parsedCommand[2]))
		return(_sendMessage(client, ERR_NOSUCHCHANNEL(parsedCommand[0])));

	Channel& channel = _getChannelbyname((parsedCommand[2]));
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
	message = client.getNickname() + " " + parsedCommand[0]+ " " + parsedCommand[1] + " " + parsedCommand[2] + "\r\n";
	std::map<int, Client>::iterator i = _Clients.find(invitedClientFd);
	channel.addInvited(i->second);
    _sendMessage(client, message);
	_sendMessage(i->second, message);
    _sendMessage(client, RPL_INVITING(getServername(), parsedCommand[1], parsedCommand[2],client.getNickname()));
    _sendMessage(i->second, RPL_INVITING(getServername(), parsedCommand[1], parsedCommand[2],client.getNickname()));		
}
