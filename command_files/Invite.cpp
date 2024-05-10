/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 16:57:13 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/10 19:18:00 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"./../inc/Includes.hpp"

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

    if (invitedClientFd == 0)
	     return(_sendMessage(client, ERR_NOSUCHNICK(_getServername(), parsedCommand[1])));
	if (channel.isMember(invitedClientFd) || channel.isInvited(invitedClientFd))
		return(_sendMessage(client, ERR_USERONCHANNEL(_getServername(), parsedCommand[1], parsedCommand[2])));
    if (!channel.isMember(client.getFd()))
		return(_sendMessage(client, ERR_NOTONCHANNEL(_getServername(), parsedCommand[2])));
	if (!channel.isOperator(client.getFd()))
		return(_sendMessage(client, ERR_CHANOPRIVSNEEDED(_getServername(), parsedCommand[2])));

	message = ":" + client.getNickname() + " INVITE " + parsedCommand[1] + " " + parsedCommand[2];
	std::map<int, Client>::iterator i = _Clients.find(invitedClientFd);
	channel.addInvited(i->second);
	//ONE of this message
    //_sendMessage(client, message);ASI ESTÁ MEJOR, NOT NECESSARY, ONLY ONE MESSAGE FOR ALL!!!!
	_sendMessage(i->second, message);
    // _sendMessage(client, RPL_INVITING(_getServername(), parsedCommand[1], parsedCommand[2],client.getNickname()));
    _sendMessage(i->second, RPL_INVITING(_getServername(), parsedCommand[1], parsedCommand[2],client.getNickname()));//lets try this!!
}
