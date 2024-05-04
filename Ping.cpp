/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 13:46:50 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/04 14:19:55 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes.hpp"

void Server::_pingServer(Client &client, std::vector<std::string> parsedCommand)
{
  //PING PapaPitufo
  
  std::string text = "";
  for (size_t j = 1;j < parsedCommand.size();j++)
	  text += " " + parsedCommand[j];
  _sendMessage(client, RPL_PONG(getServername(),text));
}
