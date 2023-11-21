/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConfigFile.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:23:54 by woliveir          #+#    #+#             */
/*   Updated: 2022/05/09 15:09:49 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParserConfigFile.hpp"

ParserConfigFile::ParserConfigFile(void)
{
	this->_nbrServers = 0;	
}

ParserConfigFile	&	ParserConfigFile::operator=(const ParserConfigFile & src)
{
	if (this != &src)
	{
		this->_servers = src.getServers();
		this->_nbrServers = src.getNbrServers();
	}
	return (*this);
}

ParserConfigFile::ParserConfigFile(const ParserConfigFile & copy)
{
	*this = copy;
	return ;
}

ParserConfigFile::~ParserConfigFile(void)
{
	this->_nbrServers = 0;
}

const std::vector<Server> &	ParserConfigFile::getServers(void) const
{
	return (this->_servers);
}

const size_t 					&	ParserConfigFile::getNbrServers(void) const
{
	return (this->_nbrServers);
}

void 	ParserConfigFile::parserConfigFile(const std::string & config_path)
{
	std::ifstream	ifs;
	std::string		line;
	std::string		servers;

	ifs.open(config_path.c_str());
	if (ifs.is_open())
	{
		while(std::getline(ifs, line))
		{
			this->removeComents(line);
			servers += line;
		}
		ifs.close();
		this->splitServers(servers);
		this->_nbrServers = this->_servers.size();
		this->setupServers();
	}
	else
		throw Error::InvalidPathServer();
}

void 	ParserConfigFile::removeComents(std::string & line)
{
	size_t pos;

	pos = line.find('#', 0);
	if (pos != std::string::npos)
		line.erase(pos, (std::string::npos - pos));
}

void	ParserConfigFile::splitServers(std::string & servers)
{
	size_t						start;
	size_t						end;

	start = 0;
	while (start < servers.size())
	{
		while ((start < servers.size()) && (std::isspace(servers[start])))
			start++;
		if (servers.compare(start, 6, "server") != 0)
			throw Error::InvalidConfigurationServer();
		start = start + 6;
		this->findStartServer(servers, start);
		end = start;
		this->findEndServer(servers, end);
		this->buildServer(servers.substr(start, (end - start + 1)));
		start = end + 1;
	}
}

void	ParserConfigFile::findStartServer(const std::string & servers, size_t & start)
{
	while ((start < servers.size()) && std::isspace(servers[start]))
		start++;
	if (servers[start] != '{')
		throw Error::InvalidConfigurationServer();
}

void	ParserConfigFile::findEndServer(const std::string & servers, size_t & end)
{
	short unsigned int scope;

	scope = 0;
	while (end < servers.size())
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
	if ((end == servers.size()) && scope)
		throw Error::InvalidConfigurationServer();
}

void		ParserConfigFile::buildServer(const std::string & server)
{
	std::vector< std::string >				splitted_server;
	Server									_server;
	std::vector< std::string >::iterator	i;

	splitted_server = Utils::split(server, std::string(" \n\t"));
	if (splitted_server.size())
	{
		i = splitted_server.begin();
		while(i != splitted_server.end())
		{
			if (((*i).compare(0 , 6, "listen") == 0) && ((i + 1) != splitted_server.end()))
			{
				if (_server.getPort().size() == 0)
					_server.setPort(++i, splitted_server);
				else
					throw Error::InvalidParameter();
			}
			else if (((*i).compare(0 , 4, "host") == 0) && ((i + 1) != splitted_server.end()))
			{
				if (_server.getHost() == 0 )
					_server.setHost(*(++i));
				else
					throw Error::InvalidParameter();
			}
			else if (((*i).compare(0, 11, "server_name") == 0) && ((i + 1) != splitted_server.end()))
			{
				if (_server.getServerName().size() == 0)
					_server.setServerName(++i, splitted_server);
				else
					throw Error::InvalidParameter();
			}
			else if (((*i).compare(0, 5, "index") == 0) && ((i + 1) != splitted_server.end()))
			{
				if (_server.getIndex().size() == 0)
					_server.setIndex(++i, splitted_server);
				else
					throw Error::InvalidParameter();
			}
			else if (((*i).compare(0, 4, "root") == 0) && ((i + 1) != splitted_server.end()))
			{
				if (_server.getRoot().size() == 0)
					_server.setRoot(*(++i));
				else
					throw Error::InvalidParameter();
			}
			else if (((*i).compare(0, 8, "location") == 0) && ((i + 1) != splitted_server.end()))
					_server.setLocation(++i, splitted_server);
			if (i != splitted_server.end())
				i++;
		}
		if (_server.getHost() == 0)
			_server.setHost("localhost;");
		if (_server.getServerName().size() == 0)
			_server.setServerNameSmart("webserver42Rio;");
		if (_server.getIndex().size() == 0)
			_server.setIndexSmart("index.html;");
		if (_server.getRoot().size() == 0)
			_server.setRoot("/;");
		if (!Utils::isFileExistAndReadable(_server.getRoot(), _server.getIndex()[0]))
			throw Error::InvalidParameter();
		if ((_server.getPort().size() == 0) || (_server.getLocation().size() == 0))
			throw Error::InvalidConfigurationServer();
		this->_servers.push_back(_server);
	}
	else
		throw Error::InvalidParameter();
}

void	ParserConfigFile::setupServers(void)
{
	size_t	i;
	size_t	j;

	i = 0;
	while(i < this->_nbrServers)
	{
		j = 0;
		this->_servers[i].setupServer();
		std::cout << "Server n: " << (i + 1) << " Created in port: ";
		while (j < this->_servers[i].getPort().size())
		{
			std::cout << this->_servers[i].getPort(static_cast<int>(j));
			j++;
			if (j < this->_servers[i].getPort().size())
				std::cout << ", ";
		}
		std::cout << std::endl;
		i++;
	}
}

void	ParserConfigFile::printServer(void) const
{
	size_t		i;
	size_t		tmp0;
	size_t		tmp1;

	i = 0;
	while (i < this->_servers.size())
	{
		std::cout << "servidor n: " << (i + 1) << std::endl;
		tmp0 = 0;
		std::cout << "listen port: " << (this->_servers[i].getPort())[tmp0];
		while (++tmp0 != (this->_servers[i].getPort()).size())
			std::cout << ", "<< (this->_servers[i].getPort())[tmp0];
		std::cout << std::endl;
		std::cout << "host: " << this->_servers[i].getHost() << std::endl;
		tmp0 = 0;
		std::cout << "server_name: " << (this->_servers[i].getServerName())[tmp0];
		while (++tmp0 != (this->_servers[i].getServerName()).size())
			std::cout << ", "<< (this->_servers[i].getServerName())[tmp0];
		std::cout << std::endl;
		tmp0 = 0;
		std::cout << "index: " << (this->_servers[i].getIndex())[tmp0];
		while (++tmp0 != (this->_servers[i].getIndex()).size())
			std::cout << ", " << (this->_servers[i].getIndex())[tmp0];
		std::cout << std::endl;
		std::cout << "root: " << this->_servers[i].getRoot() << std::endl;
		tmp0 = 0;
		std::vector<Location> indorloc = this->_servers[i].getLocation();
		while(tmp0 < (this->_servers[i].getLocation()).size())
		{
			std::cout << "Location n: " << (tmp0 + 1) << " {" << std::endl;
			std::cout << "Path: " << ((this->_servers[i].getLocation())[tmp0]).getPath() << std::endl;
			tmp1 = 0;
			std::cout << "method: " << (((this->_servers[i].getLocation())[tmp0]).getMethods())[tmp1];
			while(tmp1 < (((this->_servers[i].getLocation())[tmp0]).getMethods()).size())
				std::cout << ", " << (((this->_servers[i].getLocation())[tmp0]).getMethods())[tmp1++];
			std::cout << std::endl;
			std::cout << "}" << std::endl;
			tmp0++;
		}
		i++;
	}
}