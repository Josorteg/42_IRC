/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 18:22:54 by josorteg          #+#    #+#             */
/*   Updated: 2024/05/14 19:54:25 by mmoramov         ###   ########.fr       */
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

//welcome to colloquy
# define RPL_WELCOME(nickname, servername, hostname) (std::string(":" + servername + " 001 " + nickname + " :Welcome to the " + servername + " Network, " + nickname + "!" + hostname))
# define RPL_YOURHOST(servername,nickname) (std::string(":" + servername + " 002 " +nickname +" Your host is " + servername + ", running version 1.0"))
# define RPL_CREATED(servername, time) (std::string(":" + servername +" 003 This server was created " + time))
# define RPL_MYINFO(servername,nickname) (std::string(":" + servername + " 004 " + nickname +  " " + servername)) //<servername> <version> <available user modes> <available channel modes>"

//join
# define ERR_NOSUCHCHANNEL(channel) (std::string("403 " + channel + " :No such channel"))
# define ERR_CHANNELISFULL(nickname,channel) (std::string(":Papapitufo 471 "+ nickname + " " + channel + " :Cannot join channel (+l)"))
# define ERR_INVITEONLYCHAN(channel) (std::string("473 " + channel + " :Cannot join channel (+i)"))
# define ERR_BADCHANNELKEY(nickname,channel) (std::string("475 " + nickname + " " + channel + " :Cannot join channel (+k)")) //cambio 7/5
# define RPL_NOTOPIC(servername,channel) (std::string(":" + servername + " 331 " + channel + " :No topic is set"))
# define RPL_TOPIC(servername,channel,topic) (std::string(":" + servername + " 332 " + channel + " " + topic))
# define RPL_NAMREPLY(servername,nickname,channel,clientlist)(std::string(":" + servername + " 353 " + nickname + " = " + channel + " :" + clientlist))
# define RPL_ENDOFNAMES(channel) (std::string("366 bye " + channel + " : End of /NAMES list"))

//405 ERR_TOOMANYCHANNELS // "<channel name> :You have joined too many
                        // channels"
                // - Sent to a user when they have joined the maximum
                //   number of allowed channels and they try to join
                //   another channel.


//MODE
# define RPL_CHANNELMODEIS(servername,nickname,channel, flag)(std::string(":" + servername + " 324 " + nickname + " " + channel + " " + flag))
# define ERR_KEYSET(servername,channel) (std::string(":" + servername + " 467 " + channel + " :Channel key already set"))
# define ERR_UNKNOWNMODE(servername,channel, flag) (std::string(":" + servername + " 472 " + flag + " :is unknown mode char to me for " + channel))

/*
           RPL_BANLIST
           RPL_EXCEPTLIST                  RPL_ENDOFEXCEPTLIST
           RPL_INVITELIST                  RPL_ENDOFINVITELIST
           RPL_UNIQOPIS
*/
//"<canal> :Fin de la lista de bans del canal"
//# define RPL_ENDOFBANLIST(channel)(std::string("368 " + channel + " :Fin de la lista de bans del canal"))

//who
# define RPL_WHOREPLY(channel,nickname,hostname,servername,usernick,realname)(std::string(":"+ servername + " 352 " + nickname + " " + channel + " " + hostname + " " + servername + " " + usernick + " H :0" + realname))
# define RPL_ENDOFWHO(channel,servername, nickname)(std::string)(":" + servername + " 315 " + nickname + " " + channel + " :End of /WHO list")

//ison
# define RPL_ISON(servername, nickname, listOfClients)(std::string)(":" + servername + " 303 " + nickname + " :" + listOfClients)

//PRIVMSG
# define ERR_NORECIPIENT(servername)(std::string(":" + servername + " 411 :No recipient given (PRIVMSG)"))
# define ERR_NOTEXTTOSEND(servername)(std::string(":" + servername + " 412 :No text to send"))
# define ERR_CANNOTSENDTOCHAN(servername, channel)(std::string(":" + servername + " 412 " + channel + " :Cannot send to channel"))
# define ERR_NOSUCHNICK(servername, nickname)(std::string(":" + servername + " 401 " + nickname + " :No such nick/channel"))
    // ERR_TOOMANYTARGETS --dont know if we need it
    // RPL_AWAY --dont know if we need it

//INVITE
# define ERR_NOTONCHANNEL(servername, channel)(std::string(":" + servername + " 442 " + channel + " :You're not on that channel"))
# define ERR_USERONCHANNEL(servername, nickname, channel)(std::string(":" + servername + " 443 " + nickname + " " + channel + " :is already on channel"))
# define ERR_CHANOPRIVSNEEDED(servername, channel)(std::string(":" + servername + " 482 " + channel + " :You're not channel operator"))
# define RPL_INVITING(servername, nickname, channel,nicknameinvited)(std::string(":" + servername + " 341 " + nickname +" "+ channel + " " + nicknameinvited))

//PING
# define RPL_PONG(servername,text)(std::string(":" + servername + " 000 PONG " + text))

//KICK
# define ERR_USERNOTINCHANNEL(servername, nickname, channel)(std::string(":" + servername + " 441 " + nickname + " " + channel + " :They aren't on that channel"))
