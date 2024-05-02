/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 15:27:06 by josorteg          #+#    #+#             */
/*   Updated: 2024/05/02 17:42:43 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Channel.hpp"

Channel::Channel(Server *serv,std::string name,Client &client)
{
	this->_name = name;
	_server=serv;
	_members.insert(client.getFd());
	_invited.insert(client.getFd());
	_operators.insert(client.getFd());
	_i = false;
	_k = false;
	_l = false;
	_topic = "life";
}

bool Channel::_nameCheck(std::string name)
{
	std::string chars ="&#+!";
	std::size_t found = chars.find_first_not_of(name[0]);
	if (found == std::string::npos || name.size() > 51)
		return false;
	name.erase(name.begin());
	std::cout<<name<<std::endl;
	found = name.find_first_not_of(NICK_CHARACTERS);
	if (found != std::string::npos)
		return false;
	return true;
}
void Channel::addMember(Client client)
{
	this->_members.insert(client.getFd());
}
void Channel::addOperator(Client client)
{
	this->_operators.insert(client.getFd());
}
void Channel::addInvited(Client client)
{
	this->_invited.insert(client.getFd());
}
Channel::~Channel(void){}
