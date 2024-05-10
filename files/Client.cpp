/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 17:01:50 by josorteg          #+#    #+#             */
/*   Updated: 2024/05/10 12:12:04 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../inc/Includes.hpp"

Client::Client(void)
{

}

Client::~Client(void)
{

}

Client::Client (int fd)
{
	this->_clientFd = fd;
	this->_hasPassword = false;
	this->_isRegistered = false;
}


