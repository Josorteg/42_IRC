/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 13:46:50 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/30 00:16:36 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../inc/Includes.hpp"

void Server::_pingServer(Client &client, std::vector<std::string> parsedCommand)
{
	//PING PapaPitufo
	_sendMessage(client, RPL_PONG(_getServername(),parsedCommand[1]));
}
