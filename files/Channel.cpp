/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 15:27:06 by josorteg          #+#    #+#             */
/*   Updated: 2024/05/23 18:37:06 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../inc/Includes.hpp"

Channel::Channel(Server *serv,std::string name,Client &client)
{
	this->_name = name;
	_server=serv;
	_botActive = 0;
	_members.insert(client.getFd());
	_invited.insert(client.getFd());
	_operators.insert(client.getFd());
	_i = false;
	_k = false;
	_l = false;
	_t = false;
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


void Channel::addToBotCounter(Client client)
{
	for (std::map<int, int>::iterator it = _badWordsCounter.begin(); it != _badWordsCounter.end(); ++it)
   	{
		if (it->first == client.getFd())
		{
			it->second++;
			return;
		}
	}
	this->_badWordsCounter.insert(std::make_pair(client.getFd(), 1));
}

int Channel::getBadWordCounter(Client client)
{
	for (std::map<int, int>::iterator it = _badWordsCounter.begin(); it != _badWordsCounter.end(); ++it)
   	{
		if (it->first == client.getFd())
			return(it->second);
	}
	return 0;
}



Channel::~Channel(void){}
