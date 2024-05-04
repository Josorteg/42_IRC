/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 18:30:55 by josorteg          #+#    #+#             */
/*   Updated: 2024/05/04 18:07:41 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes.hpp"


bool sigend = false;
Server::Server (void){
	this->_servername = "PapaPitufo";
	this->setTime();
}

void Server::SetServer(int port,std::string psw)
{
	sockaddr_in serverAdress;
	pollfd NewPoll;
	this->_password = psw;
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
	for (size_t i = 0; i < _pollFds.size(); ++i)
    {
        close(_pollFds[i].fd);
        _pollFds.erase(_pollFds.begin()+i);
    }



	//FreeFds();
}

void Server::_NewClient(void)
{

	sockaddr_in clientAdress;
	pollfd NewPoll;
	socklen_t len = sizeof(clientAdress);
	int clientFd = accept(_serverFd,(struct sockaddr *)&clientAdress,&len);
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
	_Clients.insert(std::make_pair(clientFd, Client(clientFd)));
	std::cout<<"Registered a new client with fd: "<<clientFd<<std::endl;
}
void Server::_Request(int fd)
{
	char buffer[1024];
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
			//std::cout<<"Lets split a string |"<<it->second.getBuffer()<<"|"<<std::endl;
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
					//std::cout<<"buffer before reset |"<<it->second.getBuffer()<<"|"<<std::endl;
					it->second.setBuffer(""); //reset buffer
					//std::cout<<"buffer after reset |"<<it->second.getBuffer()<<"|"<<std::endl;
				}
				else
				{
					//std::cout<<"buffer before update |"<<it->second.getBuffer()<<"|"<<std::endl;
					it->second.setBuffer(commands.back()); //update buffer by last value
					//std::cout<<"buffer after update |"<<it->second.getBuffer()<<"|"<<std::endl;
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
			//std::cout<<"pushing back "<<subline<<std::endl;
		}
		start = end + delimiter.size();
	}
	subline = line.substr(start);
	if (!subline.empty())
	{
			lines.push_back(subline);//last not finished part;
			//std::cout<<"pushing last line back: "<<subline<<std::endl;
	}
	else
	{
			lines.push_back(delimiter);//if i finish i push delimiter so i know its last one.
			//std::cout<<"pushing DELIMITER back (we got a full command): "<<delimiter<<std::endl;
	}
	return lines;
}

void  Server::ProcessCommand(std::string command, int fd)
{
	std::vector<std::string> parsedCommand;
	parsedCommand = _splitString(command, ' ');
	
	std::map<int, Client>::iterator it = _Clients.find(fd);
	if (it != _Clients.end())
	{
		std::cout<<"---Hello here is time to process this command: "<<command<<"|"<<std::endl;
		if (parsedCommand[0] == "CAP")
		{
			return;
		}
		if (parsedCommand[0] == "QUIT")
		{
			_rmClient(it->second);
			return;
		}
		if (parsedCommand[0] == "PASS")//mas condiciones falta, chequear la autentificacion
		{
			_passServer(it->second,parsedCommand[1]);
			//std::cout<<"checking password"<<std::endl;
		}
		else
		{
			if (it->second.getHasPassword())
				_exe(it->second, parsedCommand);
			else
			{
				std::cout<<"Incorrect Password"<<std::endl;
				_rmClient(it->second);
			}
		}
	}
}

void Server::_rmClient(const Client &c)
{
	//needs more for
	for (size_t i = 0; i < _pollFds.size(); ++i)
    {
        if (c.getFd() == _pollFds[i].fd)
        {
            _pollFds.erase(_pollFds.begin() + i);
            break;
        }
    }
    int fd = c.getFd();
    close(fd);
	//delete _Clients[fd];/problema de malloc, no se donde alocamos memoria
	_Clients.erase(fd);
}

void Server::_passServer(Client &client,std::string pass)
{
	std::cout<<"checking password"<<std::endl;
	if (pass == getPassword())
	{
		client.setHasPassword(true);
		//client.setAutentic(true);
		std::cout<<"OK PASSWORD"<<std::endl;
	}
	else
	{
		std::cout<<"WRONG PASSWORD"<<std::endl;
		_rmClient(client);
	}

}
void Server::_exe(Client &client, std::vector<std::string> parsedCommand)
{
	std::string cmds[11] = { "USER", "NICK", "JOIN","WHO", "MODE", "PRIVMSG", "ISON", "INVITE", "TOPIC", "KICK", "PING"};
// "NAMES", };
	void	(Server::*f[11])(Client &client, std::vector<std::string> parsedCommand) = \
	{&Server::_userServer, &Server::_nickServer,  &Server::_joinServer, &Server::_whoServer, \
	 &Server::_modeServer, &Server::_privmsgServer,&Server::_isonServer,&Server::_inviteServer, \
	 &Server::_topicServer, &Server::_kickServer, &Server::_pingServer};
	 // &Server::_names,
	 //&Server::_mode, &Server::_kick, &Server::_ping };

	 for (int i = 0; i < 11; i++)
	 {
		if (parsedCommand[0] == cmds[i])
		{
			std::cout<<"going to function for command "<<cmds[i]<<std::endl;
			(this->*f[i])(client, parsedCommand);
			return ;
		}
	 }
}

void Server::setTime() {

    std::time_t currentTime = std::time(nullptr);
    std::tm* localTime = std::localtime(&currentTime);

    int year = localTime->tm_year + 1900; // years since 1900
    int month = localTime->tm_mon + 1;     // months since January (0-based)
    int day = localTime->tm_mday;          // day of the month (1-31)
    int hour = localTime->tm_hour;         // hours since midnight (0-23)
    int minute = localTime->tm_min;        // minutes after the hour (0-59)
    int second = localTime->tm_sec;        // seconds after the minute (0-61)


    std::ostringstream oss;
    oss << year << '-' << month << '-' << day << ' ' << hour << ':' << minute << ':' << second;
    this->_time = oss.str();

	//make it better
    std::cout << "Current date and time: " << _time << std::endl;
}

size_t Server::_channelExists(std::string name)
{
	for (size_t i = 0; i < _Channels.size(); ++i)
   	{
		std::cout<<"--checking channel " << _Channels[i].getName() <<std::endl;
		if (name == _Channels[i].getName())
			return(i + 1);
	}
	return(0);
}

Channel& Server::_getChannelbyname(std::string name)
{
	int i = 0;
	while (name != _Channels[i].getName())
		i++;
	return (_Channels[i]);
}

int Server::_getClientfdByName(std::string name)
{
	for (std::map<int, Client>::iterator it = _Clients.begin(); it != _Clients.end(); ++it)
   	{
		if (it->second.getNickname() == name)
			return(it->second.getFd());
	}
	return(0);
}

void Server::_sendMessage(Client &client,std::string message)
{
	message += "\r\n";
	send(client.getFd(),message.c_str(),message.size(),0);
}

void Server::_sendMessage(Channel &channel,int clientFdException, std::string message)
{
	//sends message to all clients in the channel. If client fd exception is not 0 i send to everyone except this client
	std::set<int> listOfMembers = channel.getMembers();
	message += "\r\n";
	
	for (std::set<int>::iterator it = listOfMembers.begin(); it != listOfMembers.end(); ++it)
	{
		int memberFd= *it;
		if (clientFdException == 0 || memberFd != clientFdException)
			send(memberFd,message.c_str(),message.size(),0);
	}
}

std::string Server::_getChannelMembersTxt(Channel &channel, std::string delimiter, bool withSymbol)
{
	std::set<int> listOfMembers = channel.getMembers();
	std::string listOfClients;

	for (std::set<int>::iterator i = listOfMembers.begin(); i != listOfMembers.end(); ++i)
	{
		int a= *i;
		std::map<int, Client>::iterator it = _Clients.find(a);
		
		if (withSymbol == 1)
		{
			if (channel.isOperator(it->second.getFd()))
				listOfClients += "@";
			else
				listOfClients += "+";	
		}
		listOfClients += it->second.getNickname() + delimiter;
	}
	listOfClients.erase(listOfClients.end()-1);
	return listOfClients;
}
