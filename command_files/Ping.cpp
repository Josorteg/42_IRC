/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 13:46:50 by mmoramov          #+#    #+#             */
/*   Updated: 2024/05/10 12:12:38 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"./../inc/Includes.hpp"

void Server::_pingServer(Client &client, std::vector<std::string> parsedCommand)
{
  //PING PapaPitufo

  std::string text = "";
  for (size_t j = 1;j < parsedCommand.size();j++)
	  text += " " + parsedCommand[j];
  _sendMessage(client, RPL_PONG(getServername(),text));
}
