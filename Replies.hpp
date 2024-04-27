/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 18:22:54 by josorteg          #+#    #+#             */
/*   Updated: 2024/04/27 15:34:16 by mmoramov         ###   ########.fr       */
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
# define ERR_NOSUCHCHANNEL(name) (std::string("403 " + name + " :No such channel"))
  
//473 ERR_INVITEONLYCHAN   //  "<channel> :Cannot join channel (+i)"           
// 475  ERR_BADCHANNELKEY //"<channel> :Cannot join channel (+k)"
//471 ERR_CHANNELISFULL            "<channel> :Cannot join channel (+l)"   
    
//405 ERR_TOOMANYCHANNELS // "<channel name> :You have joined too many 
                        // channels"
                // - Sent to a user when they have joined the maximum
                //   number of allowed channels and they try to join
                //   another channel.
                        
//RPL_TOPIC
