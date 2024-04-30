/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 11:38:52 by josorteg          #+#    #+#             */
/*   Updated: 2024/04/30 10:54:03 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Includes.hpp"

//USER 			josorteg 	0 			* 			Jose Miguel Ortega Sanchez
//Parameters: 	<username> <hostname> <servername> <realname>
//# define ERR_NEEDMOREPARAMS(command) (std::string("461 " + command + " :Not enough parameters"))
//# define ERR_ALREADYREGISTRED(void) (std::string("462 :You may not reregister"))

void Server::_userServer(Client &client, std::vector<std::string> parsedCommand)
{
	//check if username has 5 parameters
	if (parsedCommand.size()<5)
	{
		std::string message = ERR_NEEDMOREPARAMS(parsedCommand[0]) + "\r\n";
		send(client.getFd(),message.c_str(),message.size(),0);
		_rmClient(client);
		return;
	}
	//check if client is already registered
	if (client.getIsRegistered())
	{
		std::string message = ERR_ALREADYREGISTRED() + "\r\n";
		send(client.getFd(),message.c_str(),message.size(),0);
		_rmClient(client);
		return;
	}
	client.setUsername(parsedCommand[1]);
	client.setRealname(parsedCommand[4]);
	client.setHostname(parsedCommand[1] + "@127.0.0.1");
	client.setIsRegistered(true);

	std::string message = RPL_WELCOME(client.getNickname(), this->getServername() ,client.getHostname()) + "\r\n";
	std::cout<<"WELCOME MESSAGE "<< message<<std::endl;
	send(client.getFd(),message.c_str(),message.size(),0);

	message = RPL_YOURHOST(this->getServername(),client.getNickname()) + "\r\n";
	std::cout<<"WELCOME MESSAGE 2 "<< message<<std::endl;
	send(client.getFd(),message.c_str(),message.size(),0);


	message = RPL_CREATED(this->getServername(),this->getTime()) + "\r\n";
	std::cout<<"WELCOME MESSAGE 3 "<< message<<std::endl;
	send(client.getFd(),message.c_str(),message.size(),0);

	message = RPL_MYINFO(this->getServername(),client.getNickname()) + "\r\n";
	std::cout<<"WELCOME MESSAGE 4 "<< message<<std::endl;
	send(client.getFd(),message.c_str(),message.size(),0);



	//here i added your code,i added nickname we saved for testing 2 clients
		//to see in client
		//RPL_WELCOME(client, networkname, nick, userhost) (std::string("001 ") + client + " :Welcome to the " + networkname + " Network, " + nick + "!" + userhost)
		// std::string messagew = "001 " + it->second.getNickname() + " :Welcome to the IRCMEME Yensaika josorteg\r\n";
		// //# define RPL_YOURHOST(client, servername) (std::string("002 ") + client + " :Your host is " + servername + ", running version 1.0")
		// std::string messagey = "002 Yensaika :IRCMEME\r\n";

		// send(fd, messagew.c_str(), messagew.size(), 0);
		// send(fd, messagey.c_str(), messagey.size(), 0);
		//client->sendMessage(RPL_MYINFO(client->getNick(), "Jareste.Segfault.BieldidNothing"));
		//sendMessage(RPL_TOPIC(client->getNick(), channel->getName(), channel->getTopic()));
		//checking response
		//std::string message = "ECHO\r\n";
 		//send(fd, message.c_str(), message.size(), 0);

}
