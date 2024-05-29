/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 17:00:28 by josorteg          #+#    #+#             */
/*   Updated: 2024/05/30 00:22:31 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "./../inc/Includes.hpp"

class Client
{
	public:
		Client(void);
		Client(int fd);
		~Client(void);

		void setFd(int clientFd) {this->_clientFd = clientFd;};
		void setHasPassword(bool hasPassword) {this->_hasPassword = hasPassword;};
		void setIsRegistered(bool isRegistered){this->_isRegistered = isRegistered;};
		void setNickname(std::string nickname) {this->_nickname = nickname;};
		void setUsername(std::string username) {this->_username = username;};
		void setHostname(std::string hostname) {this->_hostname = hostname;};
		void setRealname(std::string realname) {this->_realname = realname;};
		void setBuffer(std::string buffer) {this->_buffer = buffer;};

		int getFd(void) const {return this->_clientFd;};
		bool getHasPassword(void) const {return this->_hasPassword;};
		bool getIsRegistered(void) const {return this->_isRegistered;};
		std::string getNickname(void) const {return this->_nickname;};
		std::string getUsername(void) const {return this->_username;};
		std::string getHostname(void) const {return this->_hostname;};
		std::string getRealname(void) const {return this->_realname;};
		std::string getBuffer(void) const {return this->_buffer;};

	private:
		int	_clientFd;
		bool _hasPassword;
		bool _isRegistered;
		std::string _nickname;
		std::string _username;
		std::string _hostname;
		std::string _realname;
		std::string _buffer;
};

