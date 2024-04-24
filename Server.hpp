/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 18:19:33 by josorteg          #+#    #+#             */
/*   Updated: 2024/04/24 17:58:00 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Includes.hpp"

class Client;
class Server
{
	public:

		Server(void);
		~Server(void);
		void SetServer(int port);
		void RunServer();

	private:

		void _NewClient(void);
		void _Request(int fd);


		int	_serverFd;
		//std::vector<Client> _Clients;
		std::map<int, Client> _Clients;
		std::vector<pollfd> _pollFds;

};
