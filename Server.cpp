/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 18:30:55 by josorteg          #+#    #+#             */
/*   Updated: 2024/04/17 17:56:49 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


bool sigend = false;
Server::Server (void){}
void Server::SetServer(int port)
{
	sockaddr_in serverAdress;
	pollfd NewPoll;
	_serverFd = socket(AF_INET,SOCK_STREAM,0);
	if (_serverFd == -1)
	{
		std::cerr<<"error"<<std::endl;
		exit(1);
	}
	serverAdress.sin_family = AF_INET;
	serverAdress.sin_addr.s_addr = INADDR_ANY;
	serverAdress.sin_port = htons(port);

	if (fcntl(_serverFd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr<<"error"<<std::endl;
		exit(2);
	}

	if (bind (_serverFd,(struct sockaddr *)&serverAdress,sizeof(serverAdress)) == -1)
	{
		std::cerr<<"error"<<std::endl;
		exit(3);
	}

	if (listen(_serverFd,10) == -1)
	{
		std::cerr<<"error"<<std::endl;
		exit(4);
	}

	NewPoll.fd = _serverFd;
	NewPoll.events = POLLIN;
	_pollFds.push_back(NewPoll);
}
void handler(int signal)
{
	(void)signal;
	sigend = true;
	//std::cout<<"Signal detected"<<std::endl;
}

void Server::RunServer(void)
{
	std::signal(SIGINT,handler);
	while (sigend == false)
	{
		if (poll(_pollFds.data(),_pollFds.size(),-1) == -1 && sigend == false)
		{
			std::cerr<<"errorest"<<std::endl;
			exit(5);
		}
		for (size_t it = 0; it < _pollFds.size(); it++)
		{
			if (_pollFds[it].revents & POLLIN)
			{
				if (_pollFds[it].fd == _serverFd)
					_NewClient();
				else
					_Request(_pollFds[it].fd);
			}
		}

	}
	std::cout<<"Signal detected"<<std::endl;
	//FreeFds();
}

void Server::_NewClient(void)
{

	sockaddr_in clientAdress;
	pollfd NewPoll;
	socklen_t len = sizeof(clientAdress);
	int clientFd=accept(_serverFd,(struct sockaddr *)&clientAdress,&len);
	if (clientFd == -1)
	{
		std::cerr<<"error6"<<std::endl;
			exit(6);
	}
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr<<"error7"<<std::endl;
			exit(7);
	}
	NewPoll.fd = clientFd;
	NewPoll.events = POLLIN;
	_pollFds.push_back(NewPoll);
	Client newClient(clientFd);
	_Clients.push_back(newClient);
	std::cout<<"new client with"<<clientFd<<std::endl;


}
void Server::_Request(int fd)
{
	(void)fd;
	std::cout<<"Request from:"<<fd<<std::endl;
}
Server::~Server (void){}
