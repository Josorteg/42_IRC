/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 18:22:54 by josorteg          #+#    #+#             */
/*   Updated: 2024/04/29 18:56:15 by josorteg         ###   ########.fr       */
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

//welcome

# define RPL_WELCOME(nickname, servername, hostname) (std::string("001 " + nickname + " :Welcome to the " + servername + " Network, " + nickname + "!" + hostname))
# define RPL_YOURHOST(servername) (std::string("002 Your host is " + servername + ", running version 1.0"))
# define RPL_CREATED(time) (std::string("003 This server was created " + time))
# define RPL_MYINFO(servername) (std::string("004 " + servername +  " 1.0" )) //<servername> <version> <available user modes> <available channel modes>"

//join
//ERR_NEEDMOREPARAMS
# define ERR_NOSUCHCHANNEL(channel) (std::string("403 " + channel + " :No such channel"))
# define ERR_CHANNELISFULL(channel) (std::string("471 " + channel + " :Cannot join channel (+l)"))
# define ERR_INVITEONLYCHAN(channel) (std::string("473 " + channel + " :Cannot join channel (+i)"))
# define ERR_BADCHANNELKEY(channel) (std::string("475 " + channel + " :Cannot join channel (+k)"))
# define RPL_NOTOPIC(channel) (std::string("331 " + channel + " :No topic is set"))
# define RPL_TOPIC(channel,topic) (std::string("332 " + channel + " :" + topic))
# define RPL_NAMREPLY(channel,clients) (std::string("353 bye " + channel + " " + channel +  " :"+ clients ))
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

// 221     RPL_UMODEIS
//                         "<cadena de modo de usuario>"
//# define RPL_UMODEIS()
//  324     RPL_CHANNELMODEIS
//                         "<canal> <modo> <parámetros de modo>"
# define RPL_CHANNELMODEIS(channel,modo,parameter)(std::string("324 " + channel + " " + modo + " " + parameter))
 // 368     RPL_ENDOFBANLIST
//"<canal> :Fin de la lista de bans del canal"
# define RPL_ENDOFBANLIST(channel)(std::string("368 " + channel + " :Fin de la lista de bans del canal"))

//who


# define RPL_WHOREPLY(channel,username,hostname,servername,nickuser,operator,realname)(std::string("352 bye " + channel + " " + username + " " + hostname + " " + servername + " @" + nickuser + " " operator + " :0" + realname))
# define RPL_ENDOFWHO(channel)(std::string)("315 bye " + channel + " :End of /WHO list")


