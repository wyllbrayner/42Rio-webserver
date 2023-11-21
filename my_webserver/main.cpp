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

#include "Error.hpp"
#include "ParserConfigFile.hpp"

#include <iostream>

#define DB "./configs/default.conf"
#define REQUEST "./configs/html.Request"

int	main(int argc, char **argv)
{
	ParserConfigFile	server;
	try
	{
		if (argc > 2)
			throw Error::InvalidArg();
		else
		{
			if (argc == 1)
				server.parserConfigFile(DB);
			else
				server.parserConfigFile(argv[1]);
			server.printServer();
/*
			std::vector< ConfigFile > inter = server.getServers();
			std::vector< ConfigFile >::iterator i = inter.begin();
			size_t									j = 0;
			while (j < server.getNbrServers())
			{
				std::cout << "servidor n: " << (j + 1) << " porta: " << i->getPort() << std::endl;
				j++;
				i++;
			}
			ParserRequest	requestParser;
			requestParser.parserRequest(REQUEST);
			std::cout << "Method     : " << requestParser.getMethod() << std::endl;
			std::cout << "Location   : " << requestParser.getLocation() << std::endl;
			std::cout << "RequestInf : " << requestParser.getRequestedInf() << std::endl;
			std::cout << "ContentType: " << requestParser.getContentType() << std::endl;
*/
		}
	}
	catch ( std::exception &e )
	{
		std::cerr << e.what() << std::endl;
	}
	return (0);
}
