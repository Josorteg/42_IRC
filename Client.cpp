/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 17:01:50 by josorteg          #+#    #+#             */
/*   Updated: 2024/04/25 19:15:14 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void)
{

}

Client::~Client(void)
{

}

Client::Client (int fd)
{
	this->_clientFd = fd;
	this->setUsername("BLA");
	this->_hasPassword = false;
	this->_authentic = false;
}


