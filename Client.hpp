/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 17:00:28 by josorteg          #+#    #+#             */
/*   Updated: 2024/04/27 12:28:23 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Includes.hpp"

class Client
{
	public:

		Client(void);
		Client(int fd);
		~Client(void);

		void setFd(int clientFd) {this->_clientFd = clientFd;};
		void setHasPassword(bool hasPassword) {this->_hasPassword = hasPassword;};
		void setIsRegistered(bool aut){this->_isRegistered = aut;};
		void setNickname(std::string nickname) {this->_nickname = nickname;};
		void setUsername(std::string username) {this->_username = username;};
		void setHostname(std::string hostname) {this->_hostname = hostname;};
		//void setServername(std::string servername) {this->_servername = servername;};
		void setBuffer(std::string buffer) {this->_buffer = buffer;};

		int getFd(void) const {return this->_clientFd;};
		bool getHasPassword(void) const {return this->_hasPassword;};
		bool getIsRegistered(void) const {return this->_isRegistered;};
		std::string getNickname(void) const {return this->_nickname;};
		std::string getUsername(void) const {return this->_username;};
		std::string getHostname(void) const {return this->_hostname;};
		//std::string getServername(void) const {return this->_servername;};
		std::string getBuffer(void) const {return this->_buffer;};

	private:

		int	_clientFd;
		bool _hasPassword;
		bool _isRegistered;
		std::string _nickname;
		std::string _username;
		std::string _hostname;
		//std::string _servername;
		std::string _buffer;

};

