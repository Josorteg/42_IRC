/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 15:15:44 by josorteg          #+#    #+#             */
/*   Updated: 2024/05/13 18:11:50 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"./../inc/Includes.hpp"

// if (parsedCommand[2].empty() || parsedCommand[2] == "b")
	// {
	// 	_sendMessage(client, RPL_CHANNELMODEIS(_getServername(),client.getNickname(),(parsedCommand[1])));
	// 	_sendMessage(client, RPL_ENDOFBANLIST((parsedCommand[1])));
	// }

	/*
	FLAGS WITH parameter
	MODE #example-channel +o operator_nickname
	MODE #example-channel -o operator_nickname
	MODE #example-channel +oo nickname1 nickname2

	MODE #example-channel +k password
	MODE #example-channel -k *
	MODE #42 -k password

	MODE #example-channel +l 10

	FLAGS WITHOUT parameter
	MODE #example-channel -l

	MODE #example-channel +i
	MODE #example-channel -i

	MODE #example-channel +t
	MODE #example-channel -t

	Note that there is a maximum limit of three (3) changes per command for modes that take a parameter.
	 Parameters: <channel> *( ( "-" / "+" ) *<modes> *<modeparams> )
	*/

void Server::_modeExe(Client &client, Channel &channel, std::vector<std::pair<std::string, std::string> > parsedFlags)
{
	std::string cmds[5] = {"o", "k", "l", "i", "t"};
	void (Server::*fn[5])(Client&, Channel&, std::pair<std::string, std::string>) = {
		&Server::_modeHandleOperatorPrivileges,
		&Server::_modeHandlePassword,
		&Server::_modeHandleLimit,
		&Server::_modeHandleInviteOnly,
		&Server::_modeHandleTopic
	};

	for (size_t i = 0; i < parsedFlags.size(); i++)
	{
		for (size_t j = 0; j < 5; j++)
		{
			if (parsedFlags[i].first[1] == cmds[j][0]) //example: <+o,username> - parsedFlags[i].first is "+o" and I take [1] which is "o"
			{
				(this->*fn[j])(client, channel, parsedFlags[i]);
				break;
			}
		}
	}
}

void Server::_modeServer(Client &client, std::vector<std::string> parsedCommand)
{
	std::cout<<"Before checking size"<<std::endl;
	if (parsedCommand.size() < 2)
		return(_sendMessage(client, ERR_NEEDMOREPARAMS(parsedCommand[0])));
	std::cout<<"Before checking channel exist"<<std::endl;
	if (!_channelExists((parsedCommand[1])))
		return(_sendMessage(client, ERR_NOSUCHCHANNEL(parsedCommand[1])));
	std::cout<<"Before create channel "<<std::endl;
	Channel& channel = _getChannelbyname((parsedCommand[1]));// malloc error when no channel name
	std::cout<<"after create channel exist"<<std::endl;

	if (parsedCommand.size() == 2)
	{
		std::string flags = "";
		if (channel.get_i())
			flags += "i";
		if (channel.get_k())
			flags += "k";
		if (channel.get_l())
			flags += "l";
		if (channel.get_t())
			flags += "t";

		std::cout<<"MODE all flags "<< flags << std::endl;

		_sendMessage(client, RPL_CHANNELMODEIS(_getServername(),client.getNickname(),channel.getName(),flags));
		return;
	}

    if (!channel.isMember(client.getFd())) //check if client is member of the channel
		return(_sendMessage(client, ERR_NOTONCHANNEL(_getServername(), parsedCommand[1])));
	if (!channel.isOperator(client.getFd())) //check if client is operator
		return(_sendMessage(client, ERR_CHANOPRIVSNEEDED(_getServername(), parsedCommand[1])));

	//parse data to vector of pairs
	/* for example MODE #1 ok+o-tli username password username2 50
		creates this vector: < <+o,username>,<+k,password>,<+o,username2>,<-t,empty>,<+l,50>,<+i,empty> >
	*/

	std::string flags = parsedCommand[2];
	std::vector<std::pair<std::string, std::string> > parsedFlags;
	const std::string validFlags = "oklit";
	std::string flag = "+";
	size_t paramPosition = 3;

	for (size_t i = 0; i < flags.length(); ++i)
	{
		flag = "+";
		if (flags[i] == '+' || flags[i] == '-')
			flag = flags[i++];
		if (flags[i] && validFlags.find(flags[i]) != std::string::npos)
		{
			flag += flags[i];
			if (flags[i] == 'o' || flags[i] == 'k' || (flags[i] == 'l' && flag[0] != '-'))
			{
				if (paramPosition < parsedCommand.size())
					parsedFlags.push_back(std::make_pair(flag, parsedCommand[paramPosition++]));
				else
					_sendMessage(client, ERR_NEEDMOREPARAMS(parsedCommand[0]));
			}
			else
				parsedFlags.push_back(std::make_pair(flag, "empty"));
		}
		else
		{
			flag += flags[i];
			_sendMessage(client, ERR_UNKNOWNMODE(_getServername(),parsedCommand[1],flag));
		}
	}
	//just print, (delete later)
	for (size_t i = 0; i < parsedFlags.size(); ++i)
	{
		std::cout << "Flag: " << parsedFlags[i].first << ", Parameter: " << parsedFlags[i].second << std::endl;
	}
	//now lets call proper functions
	_modeExe(client, channel, parsedFlags);
}
