/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 17:00:28 by josorteg          #+#    #+#             */
/*   Updated: 2024/04/19 20:55:33 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes.hpp"

class Client
{
	public:

		Client(void);	
		Client(int fd);
		~Client(void);

		void setFd(int clientFd) {this->_clientFd = clientFd;};
		void setHasPassword(bool hasPassword) {this->_hasPassword = hasPassword;};
		void setNickname(std::string nickname) {this->_nickname = nickname;};
		void setUsername(std::string username) {this->_username = username;};
		void setHostname(std::string hostname) {this->_hostname = hostname;};
		void setServername(std::string servername) {this->_servername = servername;};
		void setBuffer(std::string buffer) {this->_buffer = buffer;};

		int getFd(void) const {return this->_clientFd;};
		bool getHasPassword(void) const {return this->_hasPassword;};
		std::string getNickname(void) const {return this->_nickname;};
		std::string getUsername(void) const {return this->_username;};
		std::string getHostname(void) const {return this->_hostname;};
		std::string getServername(void) const {return this->_servername;};
		std::string getBuffer(void) const {return this->_buffer;};

	private:

		int	_clientFd;
		bool _hasPassword;
		std::string _nickname;
		std::string _username;
		std::string _hostname;
		std::string _servername;
		std::string _buffer;

};

