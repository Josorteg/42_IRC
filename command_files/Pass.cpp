/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 18:50:54 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/17 18:51:52 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"./../inc/Includes.hpp"

bool Server::_passServer(Client &client, std::vector<std::string> parsedCommand)
{
	std::cout<<"checking password"<<std::endl;

	if (client.getIsRegistered())
	{
		_sendMessage(client, ERR_ALREADYREGISTRED());
		return(true);
	}
	if (parsedCommand.size() < 2)
	{
		_sendMessage(client, ERR_NEEDMOREPARAMS(parsedCommand[0]));
		return(false);
	}
	if (parsedCommand[1] == _getPassword())
	{
		client.setHasPassword(true);
		std::cout<<"OK PASSWORD"<<std::endl;
		return(true);
	}
	else
	{
		std::cout<<"WRONG PASSWORD"<<std::endl;
		return(false);
	}
}