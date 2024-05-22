/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 20:05:32 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/22 20:44:29 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../inc/Includes.hpp"

void Server::_botServer(Client &client, std::vector<std::string> parsedCommand)
{
    /* !!!! call it with 4 parameters. We need also channel. Its working. Example:
       BOT SET   #test password - set on bot with correct password
       BOT UNSET #test password - unset bot with correct password
     */
    std::string password = "42";
    
    if (parsedCommand.size() < 4)
		return(_sendMessage(client, ERR_NEEDMOREPARAMS(parsedCommand[0])));
	if (parsedCommand[1] != "SET" && parsedCommand[1] != "UNSET")
		return(_sendMessage(client, ERR_NEEDMOREPARAMS((parsedCommand[2]))));    
	if (!_channelExists(parsedCommand[2]))
		return(_sendMessage(client, ERR_NOSUCHCHANNEL((parsedCommand[2]))));
         
    Channel& channel = _getChannelbyname(parsedCommand[2]);
    if (!channel.isMember(client.getFd()))  
			return(_sendMessage(client, ERR_NOTONCHANNEL(_getServername(), channel.getName())));  
    if (!channel.isOperator(client.getFd()))
	 	    return(_sendMessage(client, ERR_CHANOPRIVSNEEDED(_getServername(), channel.getName())));
    
	if (parsedCommand[3] != password)
		return(_sendMessage(client, ERR_BOT_WRONGPASS(_getServername(), client.getNickname(), channel.getName())));

    std::string message = ":" + client.getNickname() + "!" + client.getHostname() + " " + parsedCommand[0] + " " + channel.getName() + " ";
    if (parsedCommand[1] == "SET")
    {
        if (channel.getBotActive() == true)
            return(_sendMessage(client, ERR_BOT_ISACTIVE(_getServername(), client.getNickname(), channel.getName()))); 
        channel.setBotActive(true);
		message += "Bot has been set"; //todo Jose what you want 	    
    }
    else if (parsedCommand[1] == "UNSET")
    {
        channel.setBotActive(false);
        message += "Bot has been unset"; //todo Jose what you want 
    }
    _sendMessage(channel, 0, message);

}
