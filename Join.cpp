/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 14:54:15 by mmoramov          #+#    #+#             */
/*   Updated: 2024/04/27 20:46:13 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Includes.hpp"


void Server::_joinServer(Client &client, std::vector<std::string> parsedCommand)
{
   std::string message;
	//check if username has 2 parameters
	if (parsedCommand.size() < 2)
	{
		message = ERR_NEEDMOREPARAMS(parsedCommand[0]) + "\r\n";
		send(client.getFd(),message.c_str(),message.size(),0);
		_rmClient(client);
		return;
	}
	std::vector<std::string> joinChannels = _splitString(parsedCommand[1], ',');
	std::vector<std::string> keyChannels;
	if (parsedCommand.size() == 3)
		keyChannels = _splitString(parsedCommand[2], ',');
	for (size_t i = 0; i < joinChannels.size(); ++i)	
	{	
		Channel newChannel(this, joinChannels[i], client);

		if (!newChannel._nameCheck(joinChannels[i]))
		{
			message = ERR_NOSUCHCHANNEL((joinChannels[i])) + "\r\n";
			send(client.getFd(),message.c_str(),message.size(),0);
		}
		else
		{
			//parsedCommand.erase(parsedCommand.begin());
			//i check if canal exists
			std::cout<<"lets check if channel exists: " << (joinChannels[i]) <<std::endl;
			size_t nbr = _channelExists((joinChannels[i]));
			std::cout<<"found number: " << i <<std::endl;
			if (nbr)
			{
				nbr -= 1;
				//channel is full
				if (_Channels[nbr].get_l() && static_cast<int>(_Channels[nbr].getMembers().size()) == _Channels[nbr].getClientLimit())
				{
					message = ERR_CHANNELISFULL((joinChannels[i])) + "\r\n";
					send(client.getFd(),message.c_str(),message.size(),0);
				}
				//invite only channel
				 if (_Channels[nbr].get_i() && _Channels[nbr].getInvited().find(client.getFd()) != _Channels[nbr].getInvited().end())
				{
					message = ERR_INVITEONLYCHAN((joinChannels[i])) + "\r\n";
					send(client.getFd(),message.c_str(),message.size(),0);
				}
				//bad password
				else if (_Channels[nbr].get_k() && (keyChannels.size() < i \
				|| _Channels[nbr].getPassword() != keyChannels[i]))
				{
					message = ERR_BADCHANNELKEY((joinChannels[i])) + "\r\n";
					send(client.getFd(),message.c_str(),message.size(),0);
				}
				else
				{
					//create
					std::cout<<"Yes channel exists: " << (joinChannels[i]) <<std::endl;
					_Channels[nbr].addClient(client);
					message = "Added to the channel\r\n";
					send(client.getFd(),message.c_str(),message.size(),0);
				}
			}
			else
			{
				std::cout<<"No channel doesnt exists: " << (joinChannels[i]) <<std::endl;
				std::cout<<"Size before pushback: " << _Channels.size() <<std::endl;
				_Channels.push_back(newChannel);
				std::cout<<"Size after pushback: " << _Channels.size() <<std::endl;
				message = "create a new channel\r\n";
				send(client.getFd(),message.c_str(),message.size(),0); 
				nbr = _channelExists((joinChannels[i])); //find new number  							 
			}
			
			//(joinChannels[i]) = "pop";
			(joinChannels[i]).erase((joinChannels[i]).begin());
			// if JOIN is successful, the user is then sent the channel's topic
			//    (using RPL_TOPIC) and the list of users who are on the channel (using
			//    RPL_NAMREPLY), which must include the user joining.
			if (_Channels[nbr].getTopic().empty())
			{
				//i dont have topic
				message = RPL_NOTOPIC((joinChannels[i])) + "\r\n";
				send(client.getFd(),message.c_str(),message.size(),0);
			}
			else
			{
				//i have topic
				 message = RPL_TOPIC((joinChannels[i]),_Channels[nbr].getTopic()) + "\r\n";
				send(client.getFd(),message.c_str(),message.size(),0);
			}			
			message = RPL_NAMREPLY((joinChannels[i]),"@jose joan miriam") + "\r\n"; //todo
			std::cout<<"Sending: " << message <<std::endl;
			send(client.getFd(),message.c_str(),message.size(),0);
			message = RPL_ENDOFNAMES((joinChannels[i])) + "\r\n";
			std::cout<<"Sending: " << message <<std::endl;				
			send(client.getFd(),message.c_str(),message.size(),0);
		}		
		
	}
}

 