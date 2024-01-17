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

ParserServer::ParserServer(void) : _nbrServers(0) {}

ParserServer					&	ParserServer::operator=(const \
									ParserServer & src)
{
	if (this != &src)
	{
		this->_servers = src._servers;
		this->_nbrServers = src._nbrServers;
	}
	return (*this);
}

ParserServer::ParserServer(const ParserServer & copy)
{
	*this = copy;
	return ;
}

ParserServer::~ParserServer(void)
{
	this->_nbrServers = 0;
}

const std::vector<ConfigFile>	&	ParserServer::getServers(void) const
{
	return (this->_servers);
}

std::vector<ConfigFile>			&	ParserServer::getServersSmart(void)
{
	return (this->_servers);
}

const size_t					&	ParserServer::getNbrServers(void) const
{
	return (this->_nbrServers);
}

void								ParserServer::setServers(\
									std::vector<ConfigFile> & _parameter)
{
	size_t	i;

	i = 0;
	while (i < this->getNbrServers())
		this->getServersSmart()[i++].clearConfFile();
	this->_servers = _parameter;
}

void								ParserServer::createServer(void)
{
	ConfigFile	_server;
	std::string	locationStr;
	myVecS		locationVec;
	myItVecS	i;

	_server.setServerDefaultSmart(true);
	_server.setPortSmart("4242;");
	_server.setHost("127.42.42.42;");
	_server.setServerNameSmart("webserver42;");
	_server.setIndexSmart("index-example.html;");
	_server.setRoot("/;");
	_server.setMaxBodySize("1024;");
	locationStr.append("/ { }");
	locationVec = Utils::split(locationStr, std::string(" \n\t"));
	i = locationVec.begin();
	_server.setLocation(i, locationVec, _server.getRoot(), \
										_server.getIndex()[0]);
	_server.fixeErrorPage();
	this->_servers.push_back(_server);
	this->_nbrServers = this->_servers.size();
}

void								ParserServer::createServer(const \
									std::string & config_path)
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
		splitServers(servers);
		this->findReturn();
		if ((this->getServers().size() > 1) || \
				(this->getServers()[0].getPort().size() > 1))
			this->buildSingleServer();
		this->removDuplic();
		this->_nbrServers = this->_servers.size();
	}
	else
		throw Error::InvalidPathServer();
}

void								ParserServer::removeComents(\
									std::string & line)
{
	size_t	pos;

	pos = line.find('#', 0);
	if (pos != std::string::npos)
		line.erase(pos, (std::string::npos - pos));
}

void								ParserServer::splitServers(std::string \
									& servers)
{
	size_t	start;
	size_t	end;

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

void								ParserServer::findStartServer(\
									const std::string & servers, \
									size_t & start)
{
	while ((start < servers.size()) && std::isspace(servers[start]))
		start++;
	if (servers[start] != '{')
		throw Error::InvalidConfigurationServer();
}

void								ParserServer::findEndServer(\
									const std::string & servers, \
									size_t & end)
{
	short unsigned int	scope;
	
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

void								ParserServer::buildServer(\
									const std::string & server)
{
	std::vector<std::string>::iterator	i;
	std::vector<std::string>			splitted_server;
	std::string							locationStr;
	ConfigFile							_server;
	
	splitted_server = Utils::split(server, std::string(" \n\t"));
	if (splitted_server.size())
	{
		i = splitted_server.begin();
		while(i != splitted_server.end())
		{
			if (((*i).compare(0 , 6, "listen") == 0) && \
				((i + 1) != splitted_server.end()))
			{
				if (_server.getPort().empty())
					_server.setPort(++i, splitted_server);
				else
					throw Error::InvalidParameter();
			}
			else if (((*i).compare(0 , 4, "host") == 0) && \
				((i + 1) != splitted_server.end()))
			{
				if (_server.getHost() == 0)
					_server.setHost(*(++i));
				else
					throw Error::InvalidParameter();
			}
			else if (((*i).compare(0, 11, "server_name") == 0) && \
					((i + 1) != splitted_server.end()))
			{
				if (_server.getServerName().empty())
					_server.setServerName(++i, splitted_server);
				else
					throw Error::InvalidParameter();
			}
			else if (((*i).compare(0, 5, "index") == 0) && \
					((i + 1) != splitted_server.end()))
			{
				if (_server.getIndex().empty())
					_server.setIndex(++i, splitted_server);
				else
					throw Error::InvalidParameter();
			}
			else if (((*i).compare(0, 13, "max_body_size") == 0) && \
					((i + 1) != splitted_server.end()))
			{
				if (_server.getMaxBodySize() == -1)
					_server.setMaxBodySize(*(++i));
				else
					throw Error::InvalidParameter();
			}
			else if (((*i).compare(0, 4, "root") == 0) && \
					((i + 1) != splitted_server.end()))
			{
				if (_server.getRoot().empty())
					_server.setRoot(*(++i));
				else
					throw Error::InvalidParameter();
			}
			else if (((*i).compare(0, 10, "error_page") == 0) && \
					((i + 1) != splitted_server.end()))
			{
				if (_server.getErrorPage().empty())
					_server.setErrorPage(++i, splitted_server);
				else
					throw Error::InvalidParameter();
			}
			else if (((*i).compare(0, 8, "location") == 0) && \
					((i + 1) != splitted_server.end()))
			{
				if (!_server.getRoot().empty() && !_server.getIndex().empty())
				{
					_server.setLocation(++i, splitted_server, \
						_server.getRoot(), _server.getIndex()[0]);
				}
				else
					throw Error::InvalidParameter();
			}
			if (i != splitted_server.end())
				i++;
		}
		if ((_server.getPort().empty()))
			_server.setPortSmart("4242;");
		if (_server.getHost() == 0)
			_server.setHost("localhost;");
		if (_server.getRoot().empty())
			_server.setRoot("/;");
		if (_server.getServerName().empty())
			_server.setServerNameSmart("webserver42;");
		if (_server.getMaxBodySize() == -1)
			_server.setMaxBodySize("1024;");
		if (_server.getIndex().empty())
			_server.setIndexSmart("index.html;");
		if (_server.getLocation().empty())
		{
			locationStr.append("/ { }");
			splitted_server.clear();
			splitted_server = Utils::split(locationStr, std::string(" \n\t"));
			i = splitted_server.begin();
			_server.setLocation(i, splitted_server, \
					_server.getRoot(), _server.getIndex()[0]);
		}
		_server.fixeErrorPage();
		this->_servers.push_back(_server);
	}
	else
		throw Error::InvalidParameter();
}

void								ParserServer::findReturn(void)
{
	size_t	i;

	i = 0;
	while (i < this->_servers.size())
		this->getServersSmart()[i++].findLastReturn();
}

void								ParserServer::buildSingleServer(void)
{
	size_t					i;
	size_t					j;
	ConfigFile				newConfFile;
	std::vector<ConfigFile>	newServers;

	i = 0;
	while (i < this->_servers.size())
	{
		j = 0;
		while (j < this->getServersSmart()[i].getPort().size())
		{
			newConfFile.setPort(this->getServersSmart()[i].getPort()[j]);
			newConfFile.setHost(this->getServersSmart()[i].getHost());
			newConfFile.setServerName(this->getServersSmart()[i].getServerName());
			newConfFile.setIndex(this->getServersSmart()[i].getIndex());
			newConfFile.setRootSmart(this->getServersSmart()[i].getRoot());
			newConfFile.setMaxBodySize(this->getServersSmart()[i].getMaxBodySize());
			newConfFile.setErrorPage(this->getServersSmart()[i].getErrorPage());
			newConfFile.setLocation(this->getServersSmart()[i].getLocation());
			newServers.push_back(newConfFile);
			newConfFile.clearConfFile();
			j++;
		}
		i++;
	}
	this->setServers(newServers);
}

void								ParserServer::removDuplic(void)
{
	std::vector<ConfigFile>	newServers;
	bool					equal;
	size_t					i;
	size_t					j;

	i = 0;
	while (i < this->_servers.size())
	{
		equal = true;
		j = 0;
		while (j < newServers.size())
		{
			if ((this->getServers()[i].getPort()[0] == newServers[j].getPort()[0]) \
				&& (this->getServers()[i].getHost() == newServers[j].getHost()) \
				&& (this->equalServerName(i, j)) && \
				(this->getServers()[i].getRoot() == newServers[j].getRoot())) // com Root
				equal = false;
			j++;
		}
		if (equal)
			newServers.push_back(this->getServers()[i]);
		i++;
	}
	if (!newServers.empty())
		this->setServers(newServers);
}

bool								ParserServer::equalServerName(\
									const size_t & i, const size_t & j) const
{
	size_t	k;
	size_t	l;

	k = 0;
	while (k < this->getServers()[i].getServerName().size())
	{
		l = 0;
		while (l < this->getServers()[j].getServerName().size())
		{
			if (this->getServers()[i].getServerName()[k] == \
				this->getServers()[j].getServerName()[l])
				return (true);
			l++;
		}
		k++;
	}
	return (false);
}

void								ParserServer::print(void) const
{
	size_t	i;

	i = 0;
	while (i < this->_servers.size())
	{
		std::cout << "Server n\t: " << (i + 1) << std::endl;
		this->getServers()[i++].printConfigFile();
		std::cout << "###\t###\t###\t###\t###\t###\t###\t###\t###" << std::endl;
	}
}