/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 15:27:06 by josorteg          #+#    #+#             */
/*   Updated: 2024/05/04 16:32:48 by mmoramov         ###   ########.fr       */
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

void Channel::removeMember(Client client)
{
	this->_members.erase(client.getFd());
}

void Channel::removeInvited(Client client)
{
	this->_invited.erase(client.getFd());
}

void Channel::removeOperator(Client client)
{
	this->_operators.erase(client.getFd());
}

Channel::~Channel(void){}
