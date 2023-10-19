/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:23:54 by woliveir          #+#    #+#             */
/*   Updated: 2022/05/09 15:09:49 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParserServer.hpp"

ParserServer::ParserServer( void ) : _n(0) {}

ParserServer	&ParserServer::operator=( const ParserServer &src )
{
	if (this != &src)
		this->_n = src._n;
	return (*this);
}

ParserServer::ParserServer( const ParserServer& copy )
{
	*this = copy;
	return ;
}

ParserServer::~ParserServer( void )
{
	this->_n = 0;
}

void 	ParserServer::createServer( const std::string & config_path)
{
	std::ifstream	ifs;
	std::string		line;
	std::string		servers;

	ifs.open(config_path.c_str());
	if (ifs.is_open())
	{
		while( std::getline(ifs, line))
		{
//			std::cout << "file config: " << line << std::endl;
			this->removeComents( line );
//			Utils::trim( line );
			servers += line;
		}
		ifs.close();
//		std::cout << "servers final:" << servers << std::endl;
		splitServers(servers);
	}
	else
		throw Error::InvalidPathServer();
}

void 	ParserServer::removeComents( std::string & line )
{
	size_t pos;

	pos = line.find('#', 0);
	if (pos != std::string::npos)
	{
//		std::cout << "possui comentário!! Pré => line: " << line << std::endl;
		line.erase(pos, (std::string::npos - pos));
//		std::cout << "possui comentário!! Pós => line: " << line << std::endl;
	}
}

void	ParserServer::splitServers( std::string & servers )
{
	size_t						start;
	size_t						end;
	std::vector< std::string >	splitted_server;

	start = 0;
//	std::string					tmp; // apenas para teste
	std::vector< std::string >::iterator i; //para teste
	std::vector< std::string >::iterator j; //para teste
	while ( start < servers.size() )
	{
		if ( servers.compare( start, 6, "server" ) != 0 )
			throw Error::InvalidPathServer();
		start = start + 6;
		this->findStartServer( servers , start );
		end = start;
		this->findEndServer( servers , end );
//		tmp = servers.substr(start, (end - start + 1)); // apenas para teste
//		std::cout << "start: " << start << " end: " << end << " substr:" << tmp << std::endl;  // apenas para teste
		splitted_server = Utils::split(servers.substr(start, \
						(end - start + 1)), std::string(" \n\t;"));
		start = end + 1;
		if (splitted_server.size())
		{
//testeinicio
			std::cout << "splitted_server size: " << splitted_server.size() << std::endl;
			i = splitted_server.begin();
			j = splitted_server.end();
			while(i != j)
			{
				std::cout << "splitted_server: " << *i << std::endl;
				i++;
			}
//testetérmino
		}
/*
*/
	}
}

void	ParserServer::findStartServer( const std::string & servers, size_t & start )
{
	while (servers[start] && std::isspace(servers[start]))
		start++;
	if (servers[start] != '{')
		throw Error::InvalidPathServer();
}

void	ParserServer::findEndServer( const std::string & servers, size_t & end )
{
	short unsigned int scope;

	scope = 0;
	while (servers[end])
	{
		if (servers[end] == '{')
			scope++;
		else if (servers[end] == '}')
		{
			scope--;
			if (!scope)
				break ;
		}
		end++;
	}
}
