/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 18:19:33 by josorteg          #+#    #+#             */
/*   Updated: 2024/05/30 17:50:37 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "./../inc/Includes.hpp"

class Client;
class Channel;

class Server
{
	public:
		Server(void);
		~Server(void);
		void SetServer(int port,std::string psw);
		void RunServer();

	private:
		void _NewClient(void);
		int _Request(pollfd &poll);
		int _Response(pollfd &poll);
		bool _ProcessCommand(std::string command, int fd);
		std::vector<std::string> _splitString(std::string line, char delimiter);
		std::vector<std::string> _splitString(std::string line, std::string delimiter);
		std::vector<std::string> _splitString(std::string line, char delimiter, char sign);
		void _rmClient(const Client &c);

		std::string _getPassword(void){return this->_password;};
		std::string _getServername(void) const {return this->_servername;};
		std::string _getTime(void){return this->_time;};

		void _setTime(void);
		void _sendMessage(Client &client,std::string message);
		void _sendMessage(Channel &channel,int clientFdException, std::string message);
		void _exe(Client &client, std::vector<std::string> parsedCommand);
		bool _passServer(Client &client, std::vector<std::string> parsedCommand);
		bool _nickServer(Client &client, std::vector<std::string> parsedCommand);
		bool _userServer(Client &client, std::vector<std::string> parsedCommand);
		void _joinServer(Client &client, std::vector<std::string> parsedCommand);
		void _whoServer(Client &client, std::vector<std::string> parsedCommand);
		void _privmsgServer(Client &client, std::vector<std::string> parsedCommand);
		void _isonServer(Client &client, std::vector<std::string> parsedCommand);
		void _inviteServer(Client &client, std::vector<std::string> parsedCommand);
		void _topicServer(Client &client, std::vector<std::string> parsedCommand);
		void _kickServer(Client &client, std::vector<std::string> parsedCommand);
		void _pingServer(Client &client, std::vector<std::string> parsedCommand);
		void _partServer(Client &client, std::vector<std::string> parsedCommand);
		void _botServer(Client &client, std::vector<std::string> parsedCommand);

		//Channels
		size_t _channelExists(std::string name);
		Channel& _getChannelbyname(std::string name);
		int _getClientfdByName(std::string name);
		void _joinNewChannel(Client &client, std::string channelName);
		bool _joinExistingChannel(Client &client, Channel &channel);
		std::string _getChannelMembersTxt(Channel &channel,std::string delimiter, bool withSymbol);

		//MSG
		void _handleMessageToUser(Client &client, std::string receiver, std::string message);
		void _handleMessageToChannel(Client &client, std::string receiver, std::string message);

		//MODE
		void _modeServer(Client &client, std::vector<std::string> parsedCommand);
		void _modeExe(Client &client, Channel &channel, std::vector<std::pair<std::string, std::string> > parsedFlag);
		void _modeHandleOperatorPrivileges (Client &client, Channel &channel, std::pair<std::string, std::string> parsedFlag);
		void _modeHandlePassword (Client &client, Channel &channel, std::pair<std::string, std::string> parsedFlag);
		void _modeHandleLimit (Client &client, Channel &channel, std::pair<std::string, std::string> parsedFlag);
		void _modeHandleInviteOnly (Client &client, Channel &channel, std::pair<std::string, std::string> parsedFlag);
		void _modeHandleTopic (Client &client, Channel &channel, std::pair<std::string, std::string> parsedFlag);

		//KICK
		void _kickUser(Channel &channel, Client &client, std::string nickname, std::string comment);

		//BOT
		void _botchecker(Client &client, Channel &channel, std::string &message);

		std::map<int, Client> _Clients;
		std::vector<pollfd> _pollFds;
		std::vector<Channel> _Channels;

		int	_serverFd;
		std::string _password;
		std::string _servername;
		std::string _time;
};
