/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 15:27:21 by josorteg          #+#    #+#             */
/*   Updated: 2024/05/01 17:35:37 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include"Includes.hpp"

class Client;
class Server;

class Channel
{
	public:
		Channel (Server *serv,std::string name,Client &client);
		~Channel(void);
		bool _nameCheck(std::string name);
		std::string getName(void) const {return this->_name;};
		void setName(std::string name) {this->_name = name;};
		void addClient(Client client);
		std::string getPassword(void){return this->_password;};
		void setPassword(std::string pass){ this->_password = pass;};
		std::string getTopic(void){return this->_topic;};
		void setTopic(std::string topic){ this->_topic = topic;};
		bool get_i(void){return this->_i;};
		void set_i(bool i){this->_i = i;};
		bool get_k(void){return this->_k;};
		void set_k(bool k){this->_k = k;};
		bool get_l(void){return this->_l;};
		void set_l(bool l){this->_l = l;};
		int getClientLimit(void){return this->_clientLimit;};
		void setClientLimit(int limit){this->_clientLimit = limit;};
		std::set<int> getOperators(void){return this->_operators;};
		std::set<int> getInvited(void){return this->_invited;};
		std::set<int> getMembers(void){return this->_members;};
		bool isOperator(int fd) const {return _operators.find(fd) != _operators.end();};
		bool isInvited(int fd) const {return _invited.find(fd) != _invited.end();};
		bool isMember(int fd) const {return _members.find(fd) != _members.end();};

	private:

		Server *_server;
		std::string _name;
		std::string _password;
		std::string _topic;
		std::set<int> _members;
		std::set<int> _operators;
		std::set<int> _invited;

		bool	_i;
		bool	_k;
		bool	_l;
		int	_clientLimit;
};
