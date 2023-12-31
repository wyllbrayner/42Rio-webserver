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

ParserServer::ParserServer(void)
{
	this->_nbrServers = 0;	
}

ParserServer	&ParserServer::operator=(const ParserServer & src)
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

const std::vector< ConfigFile > &	ParserServer::getServers(void) const
{
	return (this->_servers);
}

std::vector< ConfigFile > &	ParserServer::getServersSmart(void)
{
	return (this->_servers);
}

const size_t 					&	ParserServer::getNbrServers(void) const
{
	return (this->_nbrServers);
}

void		ParserServer::setServers(std::vector<ConfigFile> & _parameter)
{
	size_t i;

	i = 0;
	while (i < this->getNbrServers())
		this->getServersSmart()[i++].clearConfFile();
	this->_servers = _parameter;
}

void 	ParserServer::createServer(void)
{
//	std::cout << "start | createServer (void)" << std::endl;
	ConfigFile	_server;

	_server.setPortSmart("4242;");
	_server.setHost("127.42.42.42;");
	_server.setServerNameSmart("webserver42;");
	_server.setIndexSmart("index-example.html;");
	_server.setRoot("/;");
	_server.setMaxBodySize("1024;");
	_server.fixeErrorPage();
	this->_servers.push_back(_server);
	this->_nbrServers = this->_servers.size();
//	std::cout << "end   | createServer (void)" << std::endl;
}

void 	ParserServer::createServer(const std::string & config_path)
{
//	std::cout << "start | createServer" << std::endl;
	std::ifstream	ifs;
	std::string		line;
	std::string		servers;

	ifs.open(config_path.c_str());
	if (ifs.is_open())
	{
		while(std::getline(ifs, line))
		{
//			std::cout << "file config: " << line << std::endl;
			this->removeComents(line);
//			Utils::trim( line );
			servers += line;
		}
		ifs.close();
//		std::cout << "servers final:" << servers << std::endl;
		splitServers(servers);
//		this->_nbrServers = this->_servers.size();
		this->findReturn();
		if ((this->getServers().size() > 1) || \
			(this->getServers()[0].getPort().size() > 1))
			this->buildSingleServer();
//		this->_nbrServers = this->_servers.size();
		this->removDuplic();
		this->_nbrServers = this->_servers.size();
	}
	else
		throw Error::InvalidPathServer();
//	std::cout << "end   | createServer" << std::endl;
}

void 	ParserServer::removeComents(std::string & line)
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

void	ParserServer::splitServers(std::string & servers)
{
//	std::cout << "start | splitServers" << std::endl;
	size_t	start;
	size_t	end;

	start = 0;
//	std::string					tmp; // apenas para teste
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
//		tmp = servers.substr(start, (end - start + 1)); // apenas para teste
//		std::cout << "start: " << start << " end: " << end << " substr:" << tmp << std::endl;  // apenas para teste
		this->buildServer(servers.substr(start, (end - start + 1)));
		start = end + 1;
	}
//	std::cout << "end   | splitServers" << std::endl;
}

void	ParserServer::findStartServer(const std::string & servers, size_t & start)
{
//	std::cout << "start | findStartServer | start: " << start << std::endl;

	while ((start < servers.size()) && std::isspace(servers[start]))
		start++;
	if (servers[start] != '{')
		throw Error::InvalidConfigurationServer();
//	std::cout << "end   | findStartServer | start: " << start << std::endl;
}

void	ParserServer::findEndServer(const std::string & servers, size_t & end)
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
	if ((end == servers.size()) && scope)
		throw Error::InvalidConfigurationServer();
//	std::cout << "end   | findEndServer   | end  : " << end<< std::endl;
}

void	ParserServer::buildServer(const std::string & server)
{
//	std::cout << "start | buildServer: server: " << server << std::endl;

	std::vector< std::string >				splitted_server;
	ConfigFile								_server;
	std::vector< std::string >::iterator	i;

	splitted_server = Utils::split(server, std::string(" \n\t"));
	if (splitted_server.size())
	{
		i = splitted_server.begin();
		while(i != splitted_server.end())
		{
//			std::cout << "splitted_server: " << *i << std::endl;
			if (((*i).compare(0 , 6, "listen") == 0) && ((i + 1) != splitted_server.end()))
			{
//				std::cout << "this is a listen!: " << *i << std::endl;
				if (_server.getPort().empty())
					_server.setPort(++i, splitted_server);
				else
					throw Error::InvalidParameter();
//				std::cout << "this is a listen!: " << *i << std::endl;
			}
			else if (((*i).compare(0 , 4, "host") == 0) && ((i + 1) != splitted_server.end()))
			{
//				std::cout << "this is a host!: " << *i << std::endl;
				if (_server.getHost() == 0)
					_server.setHost(*(++i));
				else
					throw Error::InvalidParameter();
//				std::cout << "this is a host!: " << *i << std::endl;
			}
			else if (((*i).compare(0, 11, "server_name") == 0) && ((i + 1) != splitted_server.end()))
			{
//				std::cout << "this is a server_name!: " << *i << std::endl;
//				if (_server.getServerName().size() == 0)
				if (_server.getServerName().empty())
					_server.setServerName(++i, splitted_server);
				else
					throw Error::InvalidParameter();
//				std::cout << "this is a server_name!: " << std::endl;
			}
			else if (((*i).compare(0, 5, "index") == 0) && ((i + 1) != splitted_server.end()))
			{
//				std::cout << "this is a index!: " << *i << std::endl;
				if (_server.getIndex().empty())
				{
//					std::cout << "index  " << *(i + 1) << std::endl;
					_server.setIndex(++i, splitted_server);
				}
				else
					throw Error::InvalidParameter();
//				std::cout << "this is a index!: " << std::endl;
			}
			else if (((*i).compare(0, 13, "max_body_size") == 0) && ((i + 1) != splitted_server.end()))
			{
//				std::cout << "this is a max_body_size!: " << *i << std::endl;
				if (_server.getMaxBodySize() == -1)
				{
//					std::cout << "max_body_size  " << *(i + 1) << std::endl;
					_server.setMaxBodySize(*(++i));
				}
				else
					throw Error::InvalidParameter();
//				std::cout << "this is a max_body_size!: " << std::endl;
			}
			else if (((*i).compare(0, 4, "root") == 0) && ((i + 1) != splitted_server.end()))
			{
//				std::cout << "this is a root: " << *i << std::endl;
				if (_server.getRoot().empty())
				{
//					std::cout << "root  " << *(i + 1) << std::endl;
					_server.setRoot(*(++i));
				}
				else
					throw Error::InvalidParameter();
//				std::cout << "this is a root: " << std::endl;
			}
			else if (((*i).compare(0, 10, "error_page") == 0) && ((i + 1) != splitted_server.end()))
			{
//				std::cout << "this is a error_page: " << *i << std::endl;
//				if (_server.getErrorPage().size() == 0)
				if (_server.getErrorPage().empty())
				{
//					std::cout << "error_page  " << *(i + 1) << std::endl;
					_server.setErrorPage(++i, splitted_server);
				}
				else
					throw Error::InvalidParameter();
//				std::cout << "this is a error_page: " << std::endl;
			}
			else if (((*i).compare(0, 8, "location") == 0) && ((i + 1) != splitted_server.end()))
			{
//				std::cout << "ini this is a location: " << *i << " com root: " << _server.getRoot() << std::endl;
				if (!_server.getRoot().empty())
				{
					_server.setLocation(++i, splitted_server, _server.getRoot());
				}
				else
					throw Error::InvalidParameter();
//				std::cout << "end this is a location: " << std::endl;
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
		if (_server.getIndex().empty()) //novo!!!!!
			_server.setIndexSmart("index.html;"); //novo!!!!!
		_server.fixeErrorPage();
		this->_servers.push_back(_server);
	}
	else
		throw Error::InvalidParameter();
//	std::cout << "end   | buildServer: server: " << server << std::endl;
}

void	ParserServer::print(void) const
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

void	ParserServer::buildSingleServer(void)
{
//	std::cout << "start | buildSingleServer" << std::endl;
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
//	std::cout << "end   | buildSingleServer" << std::endl;
}

void	ParserServer::findReturn(void)
{
//	std::cout << "Start | findReturn" <<  std::endl;
	size_t	i;

	i = 0;
	while (i < this->_servers.size())
	{
//		std::cout << "Server n: " << i << std::endl;
//		if (this->getServers()[i].getLocation().size() > 1)
		if (this->getServers()[i].getLocation().size())
		{
//			std::cout << "Server n: " << i << " possui " << this->getServers()[i].getLocation().size() << " locations. if" << std::endl;
			this->getServersSmart()[i].findLastReturn();
		}
//		else
//			std::cout << "Server n: " << i << " possui " << this->getServers()[i].getLocation().size() << " locations. else" << std::endl;	
		i++;
	}
//	std::cout << "End   | findReturn" <<  std::endl;
}

void	ParserServer::removDuplic(void)
{
//	std::cout << "Start | removDuplic" << std::endl;
	bool	equal;
	size_t	i;
	size_t	j;
	std::vector<ConfigFile>	newServers;

	i = 0;
	while (i < this->_servers.size())
	{
		equal = true;
//		std::cout << "Server n: " << (i + 1) << std::endl;
		j = 0;
		while (j < newServers.size())
		{
//			std::cout << "newServer n: " << (j + 1) << std::endl;
//			if ((this->getServers()[i].getPort()[0] == newServers[j].getPort()[0]) && (this->getServers()[i].getHost() == newServers[j].getHost()) && (this->equalServerName(i, j))) // sem Root
//			if ((this->getServers()[i].getPort()[0] == newServers[j].getPort()[0])) // com Porta
			if ((this->getServers()[i].getPort()[0] == newServers[j].getPort()[0]) && (this->getServers()[i].getHost() == newServers[j].getHost()) && (this->equalServerName(i, j)) && (this->getServers()[i].getRoot() == newServers[j].getRoot())) // com Root
			{
/*
				std::cout << "Porta de i[" << i + 1 << "]: " << this->getServers()[i].getPort()[0] << " igual à Porta de j[" << j + 1 <<"]: " << newServers[j].getPort()[0] << std::endl;
				std::cout << "Host de i[" << i + 1 << "]: " << this->getServers()[i].getHost() << " igual ao Host de j[" << j + 1 <<"]: " << newServers[j].getHost() << std::endl;
				std::cout << "Host de i[" << i + 1 << "]: " << this->getServers()[i].getHost() << " igual ao Host de j[" << j + 1 <<"]: " << newServers[j].getHost() << std::endl;
				std::cout << "ServerName de i[" << i + 1 << "] igual à ServerName de j[" << j + 1 << "]" << std::endl;
				std::cout << "Root de i[" << i + 1 << "]: " << this->getServers()[i].getRoot() << " igual ao Root de j[" << j + 1 <<"]: " << newServers[j].getRoot() << std::endl;
*/
				equal = false;
			}
			j++;
		}
		if (equal)
			newServers.push_back(this->getServers()[i]);
		i++;
	}
//	std::cout << "pré\toriginal size: " << this->_servers.size() << " newServers size: " << newServers.size() << std::endl;
	if (!newServers.empty())
		this->setServers(newServers);
//	std::cout << "pós\toriginal size: " << this->_servers.size() << " newServers size: " << newServers.size() << std::endl;
//	std::cout << "end   | removDuplic" << std::endl;
}

bool	ParserServer::equalServerName(const size_t & i, const size_t & j) const
{
//	std::cout << "start | equalServerName" << std::endl;
	size_t	k;
	size_t	l;

	k = 0;
	while (k < this->getServers()[i].getServerName().size())
	{
		l = 0;
		while (l < this->getServers()[j].getServerName().size())
		{
			if (this->getServers()[i].getServerName()[k] == this->getServers()[j].getServerName()[l])
				return (true);
			l++;
		}
		k++;
	}
//	std::cout << "end   | equalServerName" << std::endl;
	return (false);
}
