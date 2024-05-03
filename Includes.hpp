/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 17:50:48 by josorteg          #+#    #+#             */
/*   Updated: 2024/05/03 18:37:15 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <poll.h>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <csignal>
#include <set>
#include <sstream>
#include <ctime>
#include <utility>
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Replies.hpp"

# define NICK_CHARACTERS "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890[]{}\\|-"
