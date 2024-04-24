/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 15:27:06 by josorteg          #+#    #+#             */
/*   Updated: 2024/04/24 17:03:47 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Channel.hpp"

Channel::Channel(Server *serv,std::string name,Client &client)
{
	this->_name = name;
	_server=serv;
	_members.insert(c.getFd());
	_invited.insert(c.getFd());
	_operators.insert(c.getFd());


}
