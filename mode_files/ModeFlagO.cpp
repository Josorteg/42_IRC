/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeFlagO.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 19:43:48 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/30 00:21:35 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../inc/Includes.hpp"

void Server::_modeHandleOperatorPrivileges (Client &client, Channel &channel, std::pair<std::string, std::string> parsedFlag)
{
	int NewOperatorFd = _getClientfdByName(parsedFlag.second);

	if (NewOperatorFd == 0)
		return(_sendMessage(client, ERR_NOSUCHNICK(_getServername(), parsedFlag.second)));
	if (!channel.isMember(NewOperatorFd))
		return(_sendMessage(client, ERR_NOTONCHANNEL(_getServername(), channel.getName())));

	std::map<int, Client>::iterator it = _Clients.find(NewOperatorFd);

	if (parsedFlag.first[0] == '+')
		channel.addOperator(it->second);
	else
		channel.removeOperator(it->second);
	parsedFlag.first.append(" ").append(parsedFlag.second);
	_sendMessage(channel,0, RPL_CHANNELMODEIS(_getServername(),client.getNickname(),channel.getName(), parsedFlag.first));
}
