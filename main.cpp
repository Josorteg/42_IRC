/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josorteg <josorteg@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 19:08:45 by josorteg          #+#    #+#             */
/*   Updated: 2024/04/25 12:56:21 by josorteg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Server.hpp"

int main (int ac, char **arg)
{
	if (ac != 3)
		return(1);
	Server init;
	init.SetServer(atoi(arg[1]),arg[2]);
	init.RunServer();
}
