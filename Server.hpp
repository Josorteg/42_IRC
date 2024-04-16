/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 18:19:33 by josorteg          #+#    #+#             */
/*   Updated: 2024/04/16 19:48:19 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <iostream>
#include <poll.h>
#include <string>
#include <cstring>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <csignal>

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
		std::vector<pollfd> _pollFds;

};
