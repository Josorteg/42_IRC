/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 15:27:21 by josorteg          #+#    #+#             */
/*   Updated: 2024/04/24 17:08:58 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include"Includes.hpp"

class Channel
{
	public:
		Channel (Server *serv,std::string name,Client &client);
		~Channel(void);

	private:

		Server *_server;
		std::string _name;
		std::set<int> _members;
		std::set<int> _operators;
		std::set<int> _invited;




};
