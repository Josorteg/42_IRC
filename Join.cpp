/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 14:54:15 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/03 21:08:18 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Includes.hpp"


void Server::_joinServer(Client &client, std::vector<std::string> parsedCommand)
{
   std::string message;
	if (parsedCommand.size() < 2)
	{
		_sendMessage(client, ERR_NEEDMOREPARAMS(parsedCommand[0]));
		_rmClient(client);
		return;
	}
	std::vector<std::string> listOfChannels = _splitString(parsedCommand[1], ',');
	std::vector<std::string> keyChannels;
	if (parsedCommand.size() == 3)
		keyChannels = _splitString(parsedCommand[2], ',');
	for (size_t i = 0; i < listOfChannels.size(); ++i)
	{
		Channel newChannel(this, listOfChannels[i], client);
		if (!newChannel._nameCheck(listOfChannels[i]))
		{
			_sendMessage(client, ERR_NOSUCHCHANNEL((listOfChannels[i])));
		}
		else
		{
			//i check if canal exists
			size_t nbr = _channelExists((listOfChannels[i]));
			std::cout<<"found number: " << i <<std::endl;
			if (nbr)
			{
				nbr -= 1;
				//channel is full
				if (_Channels[nbr].get_l() && static_cast<int>(_Channels[nbr].getMembers().size()) == _Channels[nbr].getClientLimit())
				{
					_sendMessage(client, ERR_CHANNELISFULL((listOfChannels[i])));
				}
				//invite only channel
				if (_Channels[nbr].get_i() && _Channels[nbr].getInvited().find(client.getFd()) != _Channels[nbr].getInvited().end())
				{
					_sendMessage(client, ERR_INVITEONLYCHAN((listOfChannels[i])));
				}
				//bad password
				else if (_Channels[nbr].get_k() && (keyChannels.size() < i \
				|| _Channels[nbr].getPassword() != keyChannels[i]))
				{
					_sendMessage(client, ERR_BADCHANNELKEY((listOfChannels[i])));
				}
				else
				{
					//create
					std::cout<<"Yes channel exists: " << (listOfChannels[i]) <<std::endl;
					_Channels[nbr].addMember(client);
					std::set<int> currentUsers;
					std::string joinMessage = ":" + client.getNickname() + "!" + getServername() + " JOIN " + parsedCommand[1] + "\r\n";
					currentUsers = (getChannelbyname(parsedCommand[1])).getMembers();
					//mandamos mensajes join a todos los clientes con usuarios del canal de que hemos llegado!!
					for (std::set<int>::iterator i = currentUsers.begin(); i != currentUsers.end(); ++i)
					{
						int a= *i;
						std::map<int, Client>::iterator it = _Clients.find(a);
						//im here
						_sendMessage(it->second, joinMessage);
						//replaymessage to rejoin
						std::string replayMessage = ":" + it->second.getNickname() + "!" + getServername() + " JOIN " + parsedCommand[1] + "\r\n";
						_sendMessage(client, replayMessage);
						//who
						_whoServer(it->second,parsedCommand);


						//
						std::cout<<"Mensaje de join a canal Ya creado????"<<std::endl;

					}
					//Todos los clientes que hay, nos tienen que mandar un mensaje para entrar en nuestro chat

				}
			}
			else
			{
				std::cout<<"No channel doesnt exists: " << (listOfChannels[i]) <<std::endl;
				_Channels.push_back(newChannel);
				std::set<int> currentUsers;
				std::string joinMessage = ":" + client.getNickname() + "!" + getServername() + " JOIN " + parsedCommand[1] + "\r\n";
				currentUsers = (getChannelbyname(parsedCommand[1])).getMembers();
				for (std::set<int>::iterator i = currentUsers.begin(); i != currentUsers.end(); ++i)
				{
					std::cout<<"Mensaje de join a canal nuevo!!!!!!!!!!!!!!!!!!!"<<std::endl;
					int a= *i;
					std::map<int, Client>::iterator it = _Clients.find(a);
					_sendMessage(it->second, joinMessage);
				}
				//message = "create a new channel\r\n";
				//send(client.getFd(),message.c_str(),message.size(),0);
				nbr = _channelExists((listOfChannels[i])); //find new number
			}
			//(listOfChannels[i]).erase((listOfChannels[i]).begin());
			if (_Channels[nbr].getTopic().empty())
			{
				//i dont have topic
				_sendMessage(client, RPL_NOTOPIC(getServername(),(listOfChannels[i])));
			}
			else
			{
				//i have topic
				_sendMessage(client, RPL_TOPIC(getServername(),(listOfChannels[i]),_Channels[nbr].getTopic()));
			}
			std::string listOfClients;
			std::set<int> currentUsers2 = (getChannelbyname(parsedCommand[1])).getMembers();
			for (std::set<int>::iterator i = currentUsers2.begin(); i != currentUsers2.end(); ++i)
			{
				int a= *i;
				std::map<int, Client>::iterator it = _Clients.find(a);
				listOfClients += "@" + it->second.getNickname() + " ";
			}
			_sendMessage(client, RPL_NAMREPLY(getServername(),client.getNickname(),listOfChannels[i],listOfClients));
			_sendMessage(client, RPL_ENDOFNAMES((listOfChannels[i])));
		}

	}
}


