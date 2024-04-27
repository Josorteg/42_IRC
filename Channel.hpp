/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 15:27:21 by josorteg          #+#    #+#             */
/*   Updated: 2024/04/27 16:16:57 by mmoramov         ###   ########.fr       */
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

	private:

		Server *_server;
		std::string _name;
		std::set<int> _members;
		std::set<int> _operators;
		std::set<int> _invited;

		




};
