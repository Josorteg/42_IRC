/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoramov <mmoramov@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 19:08:45 by josorteg          #+#    #+#             */
/*   Updated: 2024/05/30 00:26:16 by mmoramov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../inc/Includes.hpp"

bool checkInput(char *argv)
{
	int len = strlen(argv);

	for (int i = 0; i < len; i++)
	{
		if (!std::isdigit(argv[i]))
			return(1);
	}
	if (atoi(argv) < 1024 || atoi(argv) > 65535)
		return(1);
	return(0);
}

int main (int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Error. Please enter 2 arguments" << std::endl;
		std::cout << "Usage: ./IRC port password. Range of port: <1024;65535>" << std::endl;
		return(1);
	}
	if (checkInput(argv[1]))
	{
		std::cout << "Error. Incorrect input." << std::endl;
		std::cout << "Usage: ./IRC port password. Range of port: <1024;65535>" << std::endl;
		return(1);
	}
	Server init;
	init.SetServer(atoi(argv[1]),argv[2]);
	init.RunServer();
}
