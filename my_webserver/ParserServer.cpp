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
//	std::cout << "start | createServer" << std::endl;

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
//	std::cout << "end   | createServer" << std::endl;
}

void 	ParserServer::removeComents( std::string & line )
{
//	std::cout << "start | removeComents" << std::endl;
	size_t pos;

	pos = line.find('#', 0);
	if (pos != std::string::npos)
	{
//		std::cout << "possui comentário!! Pré => line: " << line << std::endl;
		line.erase(pos, (std::string::npos - pos));
//		std::cout << "possui comentário!! Pós => line: " << line << std::endl;
	}
//	std::cout << "end   | removeComents" << std::endl;
}

void	ParserServer::splitServers( std::string & servers )
{
//	std::cout << "start | splitServers" << std::endl;
	size_t						start;
	size_t						end;

	start = 0;
//	std::string					tmp; // apenas para teste
	while ( start < servers.size() )
	{
		if ( servers.compare( start, 6, "server" ) != 0 )
			throw Error::InvalidConfigurationServer();
		start = start + 6;
		this->findStartServer( servers , start );
		end = start;
		this->findEndServer( servers , end );
//		tmp = servers.substr(start, (end - start + 1)); // apenas para teste
//		std::cout << "start: " << start << " end: " << end << " substr:" << tmp << std::endl;  // apenas para teste
		this->buildServer( servers.substr(start, (end - start + 1)) );
		start = end + 1;
	}
//	std::cout << "end   | splitServers" << std::endl;
}

void	ParserServer::findStartServer( const std::string & servers, size_t & start )
{
//	std::cout << "start | findStartServer | start: " << start << std::endl;

//	while (servers[start] && std::isspace(servers[start]))
	while ( (start < servers.size()) && std::isspace(servers[start]))
		start++;
	if (servers[start] != '{')
		throw Error::InvalidConfigurationServer();
//	std::cout << "end   | findStartServer | start: " << start << std::endl;
}

void	ParserServer::findEndServer( const std::string & servers, size_t & end )
{
//	std::cout << "start | findEndtServer  | end  : " << end << std::endl;
	short unsigned int scope;

	scope = 0;
	while (end < servers.size())
	{
//		std::cout << "start | findEndtServer  | end  : " << end << " while: c "<< servers[end] << " scope: " << scope << std::endl;
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
//	std::cout << "pré final da findEndServer" << std::endl;
	if ( (end == servers.size()) && scope)
		throw Error::InvalidConfigurationServer();
//	std::cout << "end   | findEndServer   | end  : " << end<< std::endl;
}

void		ParserServer::buildServer( const std::string & server )
{
	std::cout << "start | buildServer: server: " << server << std::endl;

	std::vector< std::string >				splitted_server;
	Server									_server;
	std::vector< std::string >::iterator	i; //para teste

	splitted_server = Utils::split(server, std::string(" \n\t;"));
	if (splitted_server.size())
	{
//testeinicio
		i = splitted_server.begin();
		while(i != splitted_server.end())
		{
			std::cout << "splitted_server: " << *i << std::endl;
			if ( ( (*i).compare( 0 , 6, "listen" ) == 0 ) && ((i + 1) != splitted_server.end()) )
			{
				std::cout << "this is a listen!: " << *i << std::endl;
				if (_server.getPort() == -1)
					_server.setPort(*(++i));
				std::cout << "this is a listen!: " << *i << std::endl;
			}
			i++;
		}
//testetérmino
	}
/*
*/
	std::cout << "end   | buildServer: server: " << server << std::endl;
}