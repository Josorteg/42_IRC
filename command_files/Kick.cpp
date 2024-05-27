/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 13:43:50 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/27 18:53:33 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"./../inc/Includes.hpp"

void Server::_kickServer(Client &client, std::vector<std::string> parsedCommand)
{
    //KICK <channel>{,<channel>} <user>{,<user>} [<comment>]

    //Response: The server responds to the KICK command by removing the specified user from the channel
	//and notifying all users in the channel about the kick.

	//For the message to be syntactically correct, there MUST be
   //either one channel parameter and multiple user parameter, or as many
   //channel parameters as there are user parameters.

	if (parsedCommand.size() < 3)
		return(_sendMessage(client, ERR_NEEDMOREPARAMS(parsedCommand[0]))); ///.

    std::vector<std::string> listOfChannels = _splitString(parsedCommand[1], ',');
    std::vector<std::string> listOfUsersToKick = _splitString(parsedCommand[2], ',');
	
	if ((listOfChannels.size() > 1 && listOfChannels.size() != listOfUsersToKick.size()) || (parsedCommand.size() > 3 && parsedCommand[3][0] != ':'))
		return(_sendMessage(client, ERR_NEEDMOREPARAMS(parsedCommand[0])));
	//one server to more clients
    if (listOfChannels.size() == 1)
    {
        if (!_channelExists(listOfChannels[0]))
	    	return(_sendMessage(client, ERR_NOSUCHCHANNEL(listOfChannels[0])));
        Channel& channel = _getChannelbyname(listOfChannels[0]);
        std::set<int> listOfMembers = channel.getMembers();
        if (!channel.isMember(client.getFd()))
			return(_sendMessage(client, ERR_NOTONCHANNEL(_getServername(), channel.getName())));
        if (!channel.isOperator(client.getFd()))
		    return(_sendMessage(client, ERR_CHANOPRIVSNEEDED(_getServername(), channel.getName())));
        for (size_t i = 0; i < listOfUsersToKick.size(); ++i)
            _kickUser(channel, client, listOfUsersToKick[i], parsedCommand[3]);
    }
	else // as many channel parameters as there are user parameters.
	{
		for (size_t i = 0; i < listOfChannels.size(); ++i)
		{
			if (!_channelExists(listOfChannels[i]))
			{
				_sendMessage(client, ERR_NOSUCHCHANNEL(listOfChannels[i]));
				continue;
			}
			Channel& channel = _getChannelbyname(listOfChannels[i]);
			std::set<int> listOfMembers = channel.getMembers();
			if (!channel.isMember(client.getFd()))
			{
				_sendMessage(client, ERR_NOTONCHANNEL(_getServername(), channel.getName()));
				continue;
			}
			if (!channel.isOperator(client.getFd()))
			{
				_sendMessage(client, ERR_CHANOPRIVSNEEDED(_getServername(), channel.getName()));
				continue;
			}
			_kickUser(channel, client, listOfUsersToKick[i], parsedCommand[3]);
		}
	}
}

void Server::_kickUser(Channel &channel, Client &client, std::string nickname, std::string comment)
{
	std::string message;

	int clientFdToKick = _getClientfdByName(nickname);
    if (clientFdToKick == 0)
		return(_sendMessage(client, ERR_NOSUCHNICK(_getServername(), nickname)));
    if (!channel.isMember(clientFdToKick))
    	return(_sendMessage(client, ERR_USERNOTINCHANNEL(_getServername(), nickname, channel.getName())));

//ERROR WHEN YOU KICK YOURSELF!!!
    //send message to all people in channel i kicked him
	message =  ":" + client.getNickname() + " KICK " + channel.getName() + " " + nickname + " :You have been kicked from the channel";
	if (!comment.empty())
		message += " " + comment;
	_sendMessage(channel,0,message);

	message =  ":" + client.getNickname() + " KICKED " + channel.getName() + " " + nickname + " from channel " +  channel.getName();

	//send message to the issuer that he kicked
 	_sendMessage(client, message);

	//kick him
    channel.removeMember(clientFdToKick);
	channel.removeOperator(clientFdToKick);
	channel.removeInvited(clientFdToKick);

	//if there is no one in the channel, delete channel
	if (channel.getMembers().size() < 1)
	{
		for (std::vector<Channel>::iterator it = _Channels.begin(); it != _Channels.end(); ++it)
   		{
			if (*it == channel)
			{
				this->_Channels.erase(it);
				break;
			}
		}
	}
}
