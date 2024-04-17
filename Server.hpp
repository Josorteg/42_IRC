/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 18:19:33 by josorteg          #+#    #+#             */
/*   Updated: 2024/04/17 17:45:51 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include"Client.hpp"
#include "Includes.hpp"

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
		std::vector<Client> _Clients;
		std::vector<pollfd> _pollFds;

};
