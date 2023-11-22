/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:23:54 by woliveir          #+#    #+#             */
/*   Updated: 2022/05/09 15:09:49 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"
#include "Error.hpp"
#include <iostream>

#define DB "./configs/default.conf"
#define REQUEST "./configs/html.Request"

int	main(int argc, char **argv)
{
	WebServer	webServer;
	try
	{
		if (argc > 2)
			throw Error::InvalidArg();
		else
		{
			if (argc == 1)
				webServer.manager(DB);
			else
				webServer.manager(argv[1]);
		}
	}
	catch ( std::exception &e )
	{
		std::cerr << e.what() << std::endl;
	}
	return (0);
}
