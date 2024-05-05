/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 13:43:50 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/05 19:27:01 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes.hpp"

void Server::_kickServer(Client &client, std::vector<std::string> parsedCommand)
{
    std::cout<<"Hello i am in function _kickServer "<< client.getFd() << " " << parsedCommand[0] << std::endl;


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
    std::string comment = "";
	for (size_t j = 3;j < parsedCommand.size();j++)
		comment += " " + parsedCommand[j];

	if (listOfChannels.size() > 1 && listOfChannels.size() != listOfUsersToKick.size())
		return(_sendMessage(client, ERR_NEEDMOREPARAMS(parsedCommand[0])));
	//one server to more clients
    if (listOfChannels.size() == 1)
    {
        if (!_channelExists(listOfChannels[0]))
	    	return(_sendMessage(client, ERR_NOSUCHCHANNEL(listOfChannels[0])));
        Channel& channel = _getChannelbyname(listOfChannels[0]);
        std::set<int> listOfMembers = channel.getMembers();
        if (!channel.isMember(client.getFd()))
			return(_sendMessage(client, ERR_NOTONCHANNEL(getServername(), channel.getName())));
        if (!channel.isOperator(client.getFd()))
		    return(_sendMessage(client, ERR_CHANOPRIVSNEEDED(getServername(), channel.getName())));
        for (size_t i = 0; i < listOfUsersToKick.size(); ++i)
            _kickUser(channel, client, listOfUsersToKick[i], comment);
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
				_sendMessage(client, ERR_NOTONCHANNEL(getServername(), channel.getName()));
				continue;
			}
			if (!channel.isOperator(client.getFd()))
			{
				_sendMessage(client, ERR_CHANOPRIVSNEEDED(getServername(), channel.getName()));
				continue;
			}
			_kickUser(channel, client, listOfUsersToKick[i], comment);
		}
	}
}

void Server::_kickUser(Channel &channel, Client &client, std::string nickname, std::string comment)
{
	int clientFdToKick = _getClientfdByName(nickname);
    if (clientFdToKick == 0)
		return(_sendMessage(client, ERR_NOSUCHNICK(getServername(), nickname)));
    if (!channel.isMember(clientFdToKick))
    	return(_sendMessage(client, ERR_USERNOTINCHANNEL(getServername(), nickname, channel.getName())));
	//kick him
    channel.removeMember(clientFdToKick);
	channel.removeOperator(clientFdToKick);
	channel.removeInvited(clientFdToKick);
    //send message to all people in channel i kicked him
	std::string message;
	message = "KICK " + channel.getName() + " " + nickname + " :You have been kicked from the channel";
	if (!comment.empty())
		message += " " + comment;
	_sendMessage(channel,0,message);

	message = "KICKED " + channel.getName() + " " + nickname + " from channel " +  channel.getName();

	//send message to the issuer that he kicked
 	_sendMessage(client, message);

}
