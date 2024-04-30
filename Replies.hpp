/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 18:22:54 by josorteg          #+#    #+#             */
/*   Updated: 2024/04/30 17:20:16 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

//nickname
# define ERR_ERRONEUSNICKNAME(nickname) (std::string("432 ")+ nickname +" "+ nickname + " :Erroneus nickname")
# define ERR_NICKNAMEINUSE(nickname) (std::string("433 ") + nickname + " :Nickname is already in use")
# define ERR_NICKCOLLISION(nickname) (std::string("436 ") + nickname + " :Nickname collision KILL")
# define ERR_NONICKNAMEGIVEN(void) (std::string("431 :No nickname given"))

//user
# define ERR_NEEDMOREPARAMS(command) (std::string("461 " + command + " :Not enough parameters"))
# define ERR_ALREADYREGISTRED(void) (std::string("462 :You may not reregister"))

// //welcome

// # define RPL_WELCOME(nickname, servername, hostname) (std::string("001 " + nickname + " :Welcome to the " + servername + " Network, " + nickname + "!" + hostname))
// # define RPL_YOURHOST(servername) (std::string("002 Your host is " + servername + ", running version 1.0"))
// # define RPL_CREATED(time) (std::string("003 This server was created " + time))
// # define RPL_MYINFO(servername) (std::string("004 " + servername +  " 1.0" )) //<servername> <version> <available user modes> <available channel modes>"

//welcome to colloquy
//std::string welcomeMessage = ":" + server.getName() + " 001 " + this->_nickname + " :Welcome to the IRC Network, " + this->_nickname + "!" + this->_username + "@" + intToIp(this->getAddr()) + "\r\n";
# define RPL_WELCOME(nickname, servername, hostname) (std::string(":" + servername + " 001 " + nickname + " :Welcome to the " + servername + " Network, " + nickname + "!" + hostname))
//std::string serverMessage = ":" + server.getName() + " 002 " + this->_nickname + " :Your host is " + server.getName() + ", running custom c++ homemade version\r\n";
# define RPL_YOURHOST(servername,nickname) (std::string(":" + servername + " 002 " +nickname +" Your host is " + servername + ", running version 1.0"))
//std::string createdMessage = ":" + server.getName() + " 003 " + this->_nickname + " :This server was created (Hum... I don't remember actually)\r\n";
# define RPL_CREATED(servername, time) (std::string(":" + servername +" 003 This server was created " + time))
//std::string serverInfoMessage = ":" + server.getName() + " 004 " + this->_nickname + " " + server.getName() + " MACOSX (oiwszcerkfydnx) (bciklmnoprstv)\r\n";
# define RPL_MYINFO(servername,nickname) (std::string(":" + servername + " 004 " + nickname +  " " + servername)) //<servername> <version> <available user modes> <available channel modes>"


//join
//ERR_NEEDMOREPARAMS
# define ERR_NOSUCHCHANNEL(channel) (std::string("403 " + channel + " :No such channel"))
# define ERR_CHANNELISFULL(channel) (std::string("471 " + channel + " :Cannot join channel (+l)"))
# define ERR_INVITEONLYCHAN(channel) (std::string("473 " + channel + " :Cannot join channel (+i)"))
# define ERR_BADCHANNELKEY(channel) (std::string("475 " + channel + " :Cannot join channel (+k)"))
# define RPL_NOTOPIC(channel) (std::string("331 " + channel + " :No topic is set"))
# define RPL_TOPIC(channel,topic) (std::string("332 " + channel + " :" + topic))
//msg = ":" + server.getName() + " 353 " + client.getNickname() + " = " + name + " :" + clientsList + "\r\n";
# define RPL_NAMREPLY(servername,nickname,channel,clientlist)(std::string(":" + servername + " 353 " + nickname + " = " + channel + " :" + clientlist))
//# define RPL_NAMREPLY(channel,clients) (std::string("353 bye " + channel + " " + channel +  " :"+ clients ))
# define RPL_ENDOFNAMES(channel) (std::string("366 bye " + channel + " : End of /NAMES list"))
//# define RPL_ENDOFNAMES(channel) (std::string("366 " + channel + " :End of /NAMES list"))

//405 ERR_TOOMANYCHANNELS // "<channel name> :You have joined too many
                        // channels"
                // - Sent to a user when they have joined the maximum
                //   number of allowed channels and they try to join
                //   another channel.


 // need to finish list of clients
    //       353     RPL_NAMREPLY  "<channel> :[[@|+]<nick> [[@|+]<nick> [...]]]"


//mode (inicial)


//
//OLD 324
//# define RPL_CHANNELMODEIS(channel,modo,parameter)(std::string("324 " + channel + " " + modo + " " + parameter))

//NEW 324, first response
//std::string response = ":" + server.getName() + " 324 " + client.getNickname() + " " + name[1] + " +ovimptklb\r\n";
//send(client.getFd(), response.c_str(), response.size(), 0);
//324     RPL_CHANNELMODEIS

# define RPL_CHANNELMODEIS(servername,nickname,channel)(std::string(":" + servername + " 324 " + nickname + " " + channel + " +o"))

//"<canal> :Fin de la lista de bans del canal"
# define RPL_ENDOFBANLIST(channel)(std::string("368 " + channel + " :Fin de la lista de bans del canal"))

//who

//std::string response = ":" + server.getName() + " 352 " + client.getNickname() + " " + target + " " + member->getUsername() + " " + intToIp(member->getAddr()) + " " + server.getName() + " "+ member->getNickname() + " H :0 " + member->getRealname() + "\r\n";
# define RPL_WHOREPLY(channel,nickname,hostname,servername,usernick,realname)(std::string(":"+ servername + " 352 " + nickname + " " + channel + " " + hostname + " " + servername + " " + usernick + " H :0" + realname))
//std::string end_response = ":" + server.getName() + " 315 " + client.getNickname() + " " + target + " :End of WHO list\r\n";
# define RPL_ENDOFWHO(channel,servername, nickname)(std::string)(":" + servername + " 315 " + nickname + " " + channel + " :End of /WHO list")


