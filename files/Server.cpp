/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 18:30:55 by josorteg          #+#    #+#             */
/*   Updated: 2024/05/16 17:56:52 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../inc/Includes.hpp"

bool sigend = false;

Server::Server (void)
{
	this->_servername = "PapaPitufo";
	this->_setTime();
}

void Server::SetServer(int port,std::string psw)
{
	sockaddr_in serverAdress;
	pollfd NewPoll;
	this->_password = psw;
	_serverFd = socket(AF_INET,SOCK_STREAM,0);
	int optval = 1;//chat gpt sugerencia SO_REUSEADOR hace que se pueda reutilizar antes
	setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));//chat gpt sugerencia
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
				{
					std::cout<<"Prueba LEAK DE MEMORIA antes del request"<<std::endl;
					_Request(_pollFds[it]);
				}
			}
			//POLL out
			if (_pollFds[it].revents & POLLOUT)
			{
				_Response(_pollFds[it]);
			}
			if (_pollFds[it].fd != _serverFd )
			{
				std::map<int, Client>::iterator its = _Clients.find(_pollFds[it].fd);
				if (its->second.getFd() == _pollFds[it].fd)//problema de los nulls
					std::cout << std::endl<< "RunServer: actual buffer for fd: " << _pollFds[it].fd << ": |" << its->second.getBuffer() << "|"<<std::endl<< std::endl;
			}
		}
	}

	std::cout<<"Signal detected"<<std::endl;

	for (std::map<int, Client>::iterator it = _Clients.begin(); it != _Clients.end(); ++it)
   		_rmClient(it->second);
	for (size_t i = 0; i < _pollFds.size(); ++i)
    {
        close(_pollFds[i].fd);
        _pollFds.erase(_pollFds.begin()+i);
    }
	close(_serverFd);
}

void Server::_Response(pollfd &poll)
{

	int fd = poll.fd;
	std::map<int, Client>::iterator its = _Clients.find(fd);
	if (its != _Clients.end())
	{
		std::cout<<"Buffer : "<<its->second.getBuffer()<<std::endl;
		std::vector<std::string> commands;
		std::string line = its->second.getBuffer();
		commands = _splitString(line, "\r\n");
		for (size_t i = 0; i < commands.size() - 1; ++i)
		{
			std::cout<<"Buffer to command after 0: "<<commands[0]<<std::endl;
			if(_ProcessCommand(commands[i], fd) == false)
			{
				_rmClient(its->second);
				return;
			}
		}
		//this is not working, no \r\n in commnads.back, we are splitting by \r\n
		if (commands.back() == "\r\n")
			its->second.setBuffer("");
		else
			its->second.setBuffer(commands.back());
		poll.events = POLLIN;
	}
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
// void replaceAll(std::string& str, const std::string& from, const std::string& to) {
//     size_t start_pos = 0;
//     while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
//         str.replace(start_pos, from.length(), to);
//         // Mover la posición de inicio después del reemplazo
//         start_pos += to.length();
//     }
//}

void Server::_Request(pollfd &poll)
{
	char buffer[1024];
	memset(buffer,0,sizeof(buffer));
	int fd = poll.fd;
	ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1 , 0);
	std::cout<<"Request from client with fd: "<<fd<<std::endl;
	if (bytes == 0)
	{
		std::cout<< "Client left: " << fd << std::endl;
		_rmClient(fd);
		close(fd);
	}
	else if (bytes < 0)
	{
		std::cerr<<"error disconection"<<std::endl;
		_rmClient(fd);
		close(fd);
	}
	else
	{
		buffer[bytes] ='\0';
		std::map<int, Client>::iterator it = _Clients.find(fd);

		if (it != _Clients.end())
		{
   		 	it->second.setBuffer(it->second.getBuffer().append(buffer));
			std::cout<<"Buffer : "<<it->second.getBuffer()<<std::endl;
			std::vector<std::string> commands;
			std::string line = it->second.getBuffer();
			commands = _splitString(line, "\r\n");

			if (!commands.empty())
			{
				std::cout<<"Buffer to command before: "<<commands[0]<<std::endl;
				if (commands.size() > 1)
					poll.events = POLLOUT;
			}
		}
		else
		{
			std::cout << "Client with fd " << fd << " not found." << std::endl;
		}
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

//split a string by a delimiter, but trim and if has : after space(delimiter), then everything after :(sign) is one string
std::vector<std::string>  Server::_splitString(std::string line, char delimiter, char sign)
{
	std::vector<std::string> lines;
	std::istringstream ss(line);
	std::string token;

	std::string subline;
	size_t start = 0, end;
	while ((end = line.find(delimiter, start)) != std::string::npos)
	{
		subline = line.substr(start, end-start);
		if (!subline.empty())
		{
			if (subline[0] == sign)
				break;
			else
				lines.push_back(subline);
		}
		start = end + 1;
	}
	subline = line.substr(start);
	if (!subline.empty())
		lines.push_back(subline);

	//just for print
	std::cout << "_splitString: Splitted string: " << std::endl;
	for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it)
   	{
		std::cout << "|" << *it << "|" << std::endl;
	}

	return lines;
}

// static bool	checkInit(Client *client, cmd &c)
// {
// 	if (!client->Autenticated() || c.args[0] == "PASS" ||\
// 	(!client->Registered() && c.args[0] != "USER" && c.args[0] != "NICK"))
// 		return true;
// 	return false;
// }

bool Server::_ProcessCommand(std::string command, int fd)
{
	std::vector<std::string> parsedCommand;
	parsedCommand = _splitString(command, ' ', ':');

	std::map<int, Client>::iterator it = _Clients.find(fd);
	if (it != _Clients.end())
	{
		std::cout<<"---Hello here is time to process this command: "<<command<<"|"<<std::endl;

		if (parsedCommand[0] == "CAP")
			return (true);
		if (parsedCommand[0] == "QUIT")
			return (false);
		if (parsedCommand[0] == "PASS")
		{
			if (_passServer(it->second,parsedCommand) == false)
				return(false);
			return(true);
		}
		if (!it->second.getHasPassword())
		{
			_sendMessage(it->second, "You have to first enter a password. You cannot execute command: " + parsedCommand[0]);
			return(true);
		}
		if (parsedCommand[0] == "USER")
		{
			if (_userServer(it->second,parsedCommand) == false)
				return(false);
			return(true);
		}
		if (parsedCommand[0] == "NICK")
		{
			if (_nickServer(it->second,parsedCommand) == false)
				return(false);
			return(true);
		}
		if (it->second.getIsRegistered())
			_exe(it->second, parsedCommand);
		else
			_sendMessage(it->second, "You are not registered. You cannot execute command: " + parsedCommand[0]);
	}
	return(true);
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

	std::vector<Channel> listOfChannels = _Channels;
	for (std::vector<Channel>::iterator it = listOfChannels.begin(); it != listOfChannels.end(); ++it)
	{
		Channel channel = *it;
		channel.removeMember(fd);
		channel.removeInvited(fd);
		channel.removeOperator(fd);

		//if there is no one in the channel, delete channel
		if (channel.getMembers().size() < 1)
		{
			for (std::vector<Channel>::iterator it = _Channels.begin(); it != _Channels.end(); ++it)
   			{
				if (*it == channel)
				{
					this->_Channels.erase(it);
					break;
				}
			}
		}
	}
	//delete _Clients[fd];/problema de malloc, no se donde alocamos memoria
	close(fd);
	_Clients.erase(fd);


}

bool Server::_passServer(Client &client, std::vector<std::string> parsedCommand)
{
	std::cout<<"checking password"<<std::endl;

	if (client.getIsRegistered())
	{
		_sendMessage(client, ERR_ALREADYREGISTRED());
		return(true);
	}
	if (parsedCommand.size() < 2)
	{
		_sendMessage(client, ERR_NEEDMOREPARAMS(parsedCommand[0]));
		return(false);
	}
	if (parsedCommand[1] == _getPassword())
	{
		client.setHasPassword(true);
		std::cout<<"OK PASSWORD"<<std::endl;
		return(true);
	}
	else
	{
		std::cout<<"WRONG PASSWORD"<<std::endl;
		return(false);
		//_rmClient(client);
	}
}

void Server::_exe(Client &client, std::vector<std::string> parsedCommand)
{
	std::string cmds[11] = {"JOIN","WHO", "MODE", "PRIVMSG","NOTICE", "ISON", "INVITE", "TOPIC", "KICK", "PING","PART"};

	void	(Server::*f[11])(Client &client, std::vector<std::string> parsedCommand) = \
	{&Server::_joinServer, &Server::_whoServer, \
	 &Server::_modeServer, &Server::_privmsgServer, &Server::_privmsgServer,&Server::_isonServer,&Server::_inviteServer, \
	 &Server::_topicServer, &Server::_kickServer, &Server::_pingServer,&Server::_partServer};

	 for (int i = 0; i < 11; i++)
	 {
		if (parsedCommand[0] == cmds[i])
		{
			std::cout<<"going to function for command "<<cmds[i]<<std::endl;
			(this->*f[i])(client, parsedCommand);
			return ;
		}
	 }
	 _sendMessage(client, "Command does not exists");
}

void Server::_setTime() {

    std::time_t currentTime = std::time(nullptr);
    std::tm* localTime = std::localtime(&currentTime);

    int year = localTime->tm_year + 1900;
    int month = localTime->tm_mon + 1;
    int day = localTime->tm_mday;
    int hour = localTime->tm_hour;
    int minute = localTime->tm_min;
    int second = localTime->tm_sec;

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << day << '-'
		<< std::setw(2) << std::setfill('0') << month << '-'
		<< year << ' '
		<< std::setw(2) << std::setfill('0') << hour << ':'
		<< std::setw(2) << std::setfill('0') << minute << ':'
		<< std::setw(2) << std::setfill('0') << second;
    this->_time = oss.str();
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
	size_t i = 0;
	while (i < _Channels.size() && (name != _Channels[i].getName()))
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

		if (withSymbol == 1 && channel.isOperator(it->second.getFd()))
				listOfClients += "@";
		listOfClients += it->second.getNickname() + delimiter;
	}
	listOfClients.erase(listOfClients.end()-1);
	return listOfClients;
}
