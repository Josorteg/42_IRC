/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 17:01:50 by josorteg          #+#    #+#             */
/*   Updated: 2024/05/30 00:27:12 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../inc/Includes.hpp"

Client::Client(void){}

Client::~Client(void){}

Client::Client (int fd)
{
	this->_clientFd = fd;
	this->_hasPassword = false;
	this->_isRegistered = false;
}
