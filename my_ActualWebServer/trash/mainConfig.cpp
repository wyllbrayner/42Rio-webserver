/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:23:54 by woliveir          #+#    #+#             */
/*   Updated: 2023/12/01 09:07:26 by woliveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/Error.hpp"
#include "config/ParserServer.hpp"
#include "http/ParserRequest.hpp"

#include <iostream>

#define DB "./source/config/configs/default.conf"
#define REQUEST "./source/config/configs/html.Request"

int	main( int argc, char **argv )
{
//	std::cout << "start     | main argc: " << argc << " DB:" << DB << std::endl;
/*	change test*/
	ParserServer	server;

	try
	{
//		std::cout << "try start | main" << std::endl;
		if (argc > 2)
			throw Error::InvalidArg();
		else
		{
//			std::cout << "server nbrServers pré: " << server.getNbrServers() << std::endl;
			if (argc == 1)
				server.createServer(DB);
			else
				server.createServer(argv[1]);
/*
			std::cout << "server nbrServers pós: " << server.getNbrServers() << std::endl;
			std::vector< ConfigFile > inter = server.getServers();
			std::vector< ConfigFile >::iterator i = inter.begin();
			size_t									j = 0;
			while (j < server.getNbrServers())
			{
				std::cout << "servidor n: " << (j + 1) << " porta: " << i->getPort() << std::endl;
				j++;
				i++;
			}
*/
			ParserRequest	requestParser;
			requestParser.parserRequest(REQUEST);
			// std::cout << "Method     : " << requestParser.getMethod() << std::endl;
			// std::cout << "Location   : " << requestParser.getLocation() << std::endl;
			// std::cout << "RequestInf : " << requestParser.getRequestedInf() << std::endl;
			// std::cout << "ContentType: " << requestParser.getContentType() << std::endl;
		}		
//		std::cout << "try end   | main" << std::endl;
	}
	catch ( std::exception &e )
	{
//		std::cout << "catch     | " << std::endl;
		std::cerr << e.what() << std::endl;
	}
//	std::cout << "end       | main" << std::endl;
	return (0);
}
