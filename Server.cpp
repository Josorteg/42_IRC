/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 18:30:55 by josorteg          #+#    #+#             */
/*   Updated: 2024/04/24 18:53:51 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes.hpp"


bool sigend = false;
Server::Server (void){}
void Server::SetServer(int port)
{
	sockaddr_in serverAdress;
	pollfd NewPoll;
	_serverFd = socket(AF_INET,SOCK_STREAM,0);
	if (_serverFd == -1)
	{
		std::cerr<<"error1"<<std::endl;
		exit(1);
	}
	serverAdress.sin_family = AF_INET;
	serverAdress.sin_addr.s_addr = INADDR_ANY;
	serverAdress.sin_port = htons(port);

	if (fcntl(_serverFd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr<<"error2"<<std::endl;
		exit(2);
	}

	if (bind (_serverFd,(struct sockaddr *)&serverAdress,sizeof(serverAdress)) == -1)
	{
		std::cerr<<"error3"<<std::endl;
		exit(3);
	}

	if (listen(_serverFd,10) == -1)
	{
		std::cerr<<"error4"<<std::endl;
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
			if (_pollFds[it].fd != _serverFd)
			{
				std::map<int, Client>::iterator its = _Clients.find(_pollFds[it].fd);
				std::cout << "NEW BUFFER FOR FD " << _pollFds[it].fd << ": " << its->second.getBuffer() << std::endl;
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
	_pollFds.push_back(NewPoll);\
	_Clients.insert(std::make_pair(clientFd, Client(clientFd)));
	std::cout<<"new client with"<<clientFd<<std::endl;
	//to see in client
	//RPL_WELCOME(client, networkname, nick, userhost) (std::string("001 ") + client + " :Welcome to the " + networkname + " Network, " + nick + "!" + userhost)

	//# define RPL_YOURHOST(client, servername) (std::string("002 ") + client + " :Your host is " + servername + ", running version 1.0")
	std::string messagey = "002 Yensaika :IRCMEME\r\n";
 	send(clientFd, messagey.c_str(), messagey.size(), 0);
	//client->sendMessage(RPL_MYINFO(client->getNick(), "Jareste.Segfault.BieldidNothing"));

	//sendMessage(RPL_TOPIC(client->getNick(), channel->getName(), channel->getTopic()));



}
void Server::_Request(int fd)
{
	char buffer[10];
	memset(buffer,0,sizeof(buffer));

	ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1 , 0);

	//we add buffer to _Clients.

	std::cout<<"Request from:"<<fd<<std::endl;

	if (bytes == 0) // 0 y -1 no es lo mismo, 0 es desconexion, -1 error. De momento nada
	{
		std::cout<<"Client left: "<<fd<<std::endl;
		//funcion para liberar FD's (poll, lista de clientes y cerrrar FD)
		close(fd);
	}
	if (bytes < 0) // 0 y -1 no es lo mismo, 0 es desconexion, -1 error. De momento nada
	{
		std::cerr<<"error disconection"<<std::endl;
		//funcion para liberar FD's (poll, lista de clientes y cerrrar FD)
		close(fd);
	}
	else
	{
		buffer[bytes] ='\0';
		std::map<int, Client>::iterator it = _Clients.find(fd);
		if (it != _Clients.end()) {
   		 	it->second.setBuffer(it->second.getBuffer()+buffer);

   		 } else {
			std::cout << "Client with fd " << fd << " not found." << std::endl;
		}


		std::string messagew = "001 papapitufo :Welcome to the IRCMEME Yensaika josorteg\r\n";
 		send(fd, messagew.c_str(), messagew.size(), 0);
		//checking response
		std::string message = "ECHO\r\n";
 		send(fd, message.c_str(), message.size(), 0);
	}
}
Server::~Server (void){}
