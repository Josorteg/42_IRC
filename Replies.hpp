/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 18:22:54 by josorteg          #+#    #+#             */
/*   Updated: 2024/04/25 19:20:08 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# define ERR_NONICKNAMEGIVEN(void) (std::string("431 ") + " :No nickname given")
# define ERR_ERRONEUSNICKNAME(username, nickname) (std::string("432 ")+ username + " "+ nickname + " :Erroneus nickname")
# define ERR_NICKNAMEINUSE(nickname) (std::string("433 ") + nickname + " :Nickname is already in use")
# define ERR_NICKCOLLISION(nickname) (std::string("436 ") + nickname + " :Nickname collision KILL")
