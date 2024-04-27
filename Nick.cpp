/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 10:04:35 by josorteg          #+#    #+#             */
/*   Updated: 2024/04/27 11:53:01 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Includes.hpp"

static bool	parseNick(std::string nick)
{
	std::size_t found = nick.find_first_not_of(NICK_CHARACTERS);
	if (found != std::string::npos)
		return true;
	return false;
}
void Server::_nickServer(Client &client, std::vector<std::string> parsedCommand)
{
	//check if Nick
	if (parsedCommand.size()<2)
	{
		std::string message = ERR_NONICKNAMEGIVEN() + "\r\n";
		//ERR_NONICKNAMEGIVEN(void) (std::string("431 ") + " :No nickname given")
		send(client.getFd(),message.c_str(),message.size(),0);
		_rmClient(client);
		return;
	}
	std::cout<<"Antdes del Error en caracteres del NIck"<<std::endl;
	//check characters
	if (parseNick(parsedCommand[1]))
	{
		std::cout<<"Error en caracteres del NIck"<<std::endl;
		std::string message = ERR_ERRONEUSNICKNAME(parsedCommand[1]) + "\r\n";
		//ERR_ERRONEUSNICKNAME(username, nickname) (std::string("432 ")+ username + " "+ nickname + " :Erroneus nickname")
		send(client.getFd(),message.c_str(),message.size(),0);
		_rmClient(client);
		return;
	}
	//check nick in server
		// std::cout<<"_nickServer:  " <<std::endl;
		// std::string messagew = ERR_NICKNAMEINUSE(parsedCommand[1]) + "\r\n";
		// std::cout<< messagew <<std::endl;
		// //send(client.getFd(), messagew.c_str(), messagew.size(), 0);
		//send(client.getFd(), "432 josemiguel pepe :Erroneus nickname\r\n",41, 0);
	client.setNickname(parsedCommand[1]);
	if (client.getIsRegistered())
	{
		std::string message = "Nick change to : " + client.getNickname()+ "\r\n";
		//ERR_ERRONEUSNICKNAME(username, nickname) (std::string("432 ")+ username + " "+ nickname + " :Erroneus nickname")
		send(client.getFd(),message.c_str(),message.size(),0);
	}


		//NOthing


}
