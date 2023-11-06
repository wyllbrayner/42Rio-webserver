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
#include "ParserServer.hpp"

#include <iostream>

#define DB "./configs/default.conf"

int	main( int argc, char **argv )
{
	std::cout << "start     | main argc: " << argc << " argv[0]: " << argv[0] << std::endl;
	ParserServer	server;

	try
	{
//		std::cout << "try start | main" << std::endl;
		if (argc > 2)
			throw Error::InvalidArg();
		else
		{
			std::cout << "server nbrServers pré: " << server.getNbrServers() << std::endl;
			if (argc == 1)
				server.createServer(DB);
			else
				server.createServer(argv[1]);
			std::cout << "server nbrServers pós: " << server.getNbrServers() << std::endl;
			std::vector< Server > inter = server.getServers();
			std::vector< Server >::iterator i = inter.begin();
			size_t									j = 0;
			while (j < server.getNbrServers())
			{
				std::cout << "servidor n: " << (j + 1) << " porta: " << i->getPort() << std::endl;
				j++;
				i++;
			}
		}		
//		std::cout << "try end   | main" << std::endl;
	}
	catch ( std::exception &e )
	{
//		std::cout << "catch     | " << std::endl;
		std::cerr << e.what() << std::endl;
	}
/*
	std::string	Request = "GET /index.html HTTP/1.1Host: www.example.reUser-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.0; en-US; rv:1.1)Accept: text/htmlAccept-Language: en-US, en; q=0.5Accept-Encoding: gzip, deflate";
	size_t pos;
	std::string	tmp;
	pos = Request.find(" HTTP/");
	std::cout << "Para request: " << Request << std::endl;
	if (pos == std::string::npos)
		std::cout << "Erro na requisição" << std::endl;
	else
	{
		tmp = Request.substr(0, pos);
		std::cout << "HTTP/ na posição: " << pos << " request to parse: " << tmp << std::endl;
	}
*/
	std::cout << "end       | main" << std::endl;
	return (0);
}
