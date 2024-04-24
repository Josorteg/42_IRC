/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 18:30:55 by josorteg          #+#    #+#             */
/*   Updated: 2024/04/24 21:34:47 by mmoramov         ###   ########.fr       */
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
				std::cout << std::endl<< "RunServer: actual buffer for fd: " << _pollFds[it].fd << ": |" << its->second.getBuffer() << "|"<<std::endl<< std::endl;
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
	std::cout<<"Registered a new client with fd: "<<clientFd<<std::endl;
}
void Server::_Request(int fd)
{
	char buffer[10];
	memset(buffer,0,sizeof(buffer));

	ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1 , 0);

	//we add buffer to _Clients.

	std::cout<<"Request from client with fd: "<<fd<<std::endl;

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
		std::map<int, Client>::iterator it = _Clients.find(fd); // find a proper client by fd
		if (it != _Clients.end()) //if i found client by fd
		{
   		 	it->second.setBuffer(it->second.getBuffer().append(buffer)); //fill a clients buffer

			std::vector<std::string> commands; //vector of commands
			std::cout<<"Lets split a string |"<<it->second.getBuffer()<<"|"<<std::endl;
			commands = _splitString(it->second.getBuffer(), "\r\n"); // split the commands into vector
			
			if (!commands.empty())
			{
				for (size_t i = 0; i < commands.size() - 1; ++i) //process each command (commands.size() - 1 because last one is \r\n or not full command yet)
				{
					ProcessCommand(commands[i], fd); // Jose you can continue in this funtion :)
				}	
				//if the last one is delimiter, i need to reset buffer
				if (commands.back() == "\r\n")
				{
					std::cout<<"buffer before reset |"<<it->second.getBuffer()<<"|"<<std::endl;
					it->second.setBuffer(""); //reset buffer 
					std::cout<<"buffer after reset |"<<it->second.getBuffer()<<"|"<<std::endl;
				}
				else
				{
					std::cout<<"buffer before update |"<<it->second.getBuffer()<<"|"<<std::endl;
					it->second.setBuffer(commands.back()); //update buffer by last value
					std::cout<<"buffer after update |"<<it->second.getBuffer()<<"|"<<std::endl;
				}		
			}
   		}
		else
		{
			std::cout << "Client with fd " << fd << " not found." << std::endl;
		}
		std::cout<<std::endl; //newline
	}
}
Server::~Server (void){}

//split a string by one character
std::vector<std::string>  Server::_splitString(std::string line, char delimiter)
{
	std::vector<std::string> lines;
	std::istringstream ss(line);
	std::string token;
	while (std::getline(ss, token, delimiter))
	{
		lines.push_back(token);
	}
	return lines;
}

//split a string by string
std::vector<std::string>  Server::_splitString(std::string line, std::string delimiter)
{	
	std::vector<std::string> lines;
	std::string subline;
	size_t start = 0, end;
	while ((end = line.find(delimiter, start)) != std::string::npos)
	{
		subline = line.substr(start, end-start);
		if (!subline.empty())
		{
			lines.push_back(subline);
			std::cout<<"pushing back "<<subline<<std::endl;
		}
		start = end + delimiter.size();
	}	
	subline = line.substr(start);
	if (!subline.empty())
	{
			lines.push_back(subline);//last not finished part;
			std::cout<<"pushing last line back: "<<subline<<std::endl;
	}
	else
	{
			lines.push_back(delimiter);//if i finish i push delimiter so i know its last one.
			std::cout<<"pushing DELIMITER back (we got a full command): "<<delimiter<<std::endl;
	}
	return lines;
}

void  Server::ProcessCommand(std::string command, int fd)
{	
	//hi Jose this is for you, here you have as an input whole 1 command in line 

	std::vector<std::string> parsedCommand; //vector of commands
	parsedCommand = _splitString(command, ' ');  // i also split here command the vector by space, so we can easily check any part of command if necessary
	
	std::cout<<"---fd: "<< fd;
	std::cout<<"---Hello here is time to process this command: "<<command<<std::endl;
	std::cout<<"---and here is first word of this command: "<<parsedCommand[0]<<std::endl;

	//find client again 
	std::map<int, Client>::iterator it = _Clients.find(fd);
	if (it != _Clients.end())
	{
			
		//for now i only saved nickname
		if (parsedCommand[0] == "NICK")
		{
			it->second.setNickname(parsedCommand[1]);
		}
			
	//here i added your code,i added nickname we saved for testing 2 clients
		//to see in client
		//RPL_WELCOME(client, networkname, nick, userhost) (std::string("001 ") + client + " :Welcome to the " + networkname + " Network, " + nick + "!" + userhost)
		std::string messagew = "001 " + it->second.getNickname() + " :Welcome to the IRCMEME Yensaika josorteg\r\n";
		//# define RPL_YOURHOST(client, servername) (std::string("002 ") + client + " :Your host is " + servername + ", running version 1.0")
		std::string messagey = "002 Yensaika :IRCMEME\r\n";
		
		send(fd, messagew.c_str(), messagew.size(), 0);
		send(fd, messagey.c_str(), messagey.size(), 0);
		//client->sendMessage(RPL_MYINFO(client->getNick(), "Jareste.Segfault.BieldidNothing"));
		//sendMessage(RPL_TOPIC(client->getNick(), channel->getName(), channel->getTopic()));	
		//checking response
		std::string message = "ECHO\r\n";
 		send(fd, message.c_str(), message.size(), 0);
	}
}