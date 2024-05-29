/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 15:27:21 by josorteg          #+#    #+#             */
/*   Updated: 2024/05/30 00:22:44 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "./../inc/Includes.hpp"

class Client;
class Server;

class Channel
{
	public:
		Channel (Server *serv,std::string name,Client &client);
		bool operator==(const Channel& other) const {return this->getName() == other.getName();};
		~Channel(void);

		std::string getName(void) const {return this->_name;};
		std::string _getPassword(void){return this->_password;};
		std::string getTopic(void){return this->_topic;};
		int getClientLimit(void){return this->_clientLimit;};

		void setName(std::string name) {this->_name = name;};
		void setPassword(std::string pass){ this->_password = pass;};
		void setTopic(std::string topic){ this->_topic = topic;};
		void setClientLimit(int limit){this->_clientLimit = limit;};
		bool get_i(void){return this->_i;};
		bool get_k(void){return this->_k;};
		bool get_l(void){return this->_l;};
		bool get_t(void){return this->_t;};
		void set_i(bool i){this->_i = i;};
		void set_k(bool k){this->_k = k;};
		void set_l(bool l){this->_l = l;};
		void set_t(bool t){this->_t = t;};

		void addMember(Client client);
		void addOperator(Client client);
		void addInvited(Client client);
		void removeMember(Client client);
		void removeOperator(Client client);
		void removeInvited(Client client);
		std::set<int> getOperators(void){return this->_operators;};
		std::set<int> getInvited(void){return this->_invited;};
		std::set<int> getMembers(void){return this->_members;};
		bool isOperator(int fd) const {return _operators.find(fd) != _operators.end();};
		bool isInvited(int fd) const {return _invited.find(fd) != _invited.end();};
		bool isMember(int fd) const {return _members.find(fd) != _members.end();};
		bool getBotActive(void){return this->_botActive;};
		void setBotActive(bool botActive){this->_botActive = botActive;};
		void addToBotCounter(Client client);
		int getBadWordCounter(Client client);

	private:
		Server *_server;
		std::string _name;
		std::string _password;
		std::string _topic;
		std::set<int> _members;
		std::set<int> _operators;
		std::set<int> _invited;
		bool _botActive;
		std::map<int, int> _badWordsCounter;

		bool	_i;
		bool	_k;
		bool	_l;
		bool	_t;
		int	_clientLimit;
};
