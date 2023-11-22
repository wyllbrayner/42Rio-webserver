/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:23:54 by woliveir          #+#    #+#             */
/*   Updated: 2022/05/09 15:09:49 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

WebServer::WebServer(void)
{
	this->_biggestSocket = 0;
	FD_ZERO(&this->_recvSocketSet);
	FD_ZERO(&this->_writeSocketSet);
}

WebServer	&WebServer::operator=(const WebServer &src)
{
	if (this != &src)
	{
		return (*this);
	}
	else
		return (*this);
}

WebServer::WebServer(const WebServer& copy)
{
	*this = copy;
	return ;
}

WebServer::~WebServer(void)
{
	this->_biggestSocket = 0;
	FD_ZERO(&this->_recvSocketSet);
	FD_ZERO(&this->_writeSocketSet);
}

void	WebServer::manager(const std::string & config_path)
{
	this->_configFile.parserConfigFile(config_path);
	this->_nbrServers = this->_configFile.getNbrServers();
	this->_cluster = this->buildCluster(this->_configFile.getServers());
/*
//	this->_cluster = this->_configFile.getServers();
	this->setupCluster();
	this->initSets();
	this->runCluster();
	this->printCluster();
*/
}

const std::vector<Server> &	WebServer::buildCluster(const std::vector<Server> & _servers)
{
	size_t					i;
	size_t					j;
	std::vector<Server>		cluster;

	i = 0;
	while (i < this->_nbrServers)
	{
		j = 0;
		while (j < _servers[i].getPort().size())
		{
			std::cout << "servidor: " << i << " porta: " << _servers[i].getPort()[j] << std::endl; this->fillServer(cluster, _servers[i], j);
			j++;
		}
		i++;
	}
	std::cout << "size: " << cluster.size() << std::endl;
	return (_servers);
}

void	WebServer::fillServer(std::vector<Server> & cluster, \
		const Server & _server, const size_t & _j)
{
	Server	serverIndor;

	std::cout << "fillServer" << std::endl;
	serverIndor.setPort(_server.getPort(_j));
	std::cout << "port			: " << _server.getPort(_j) << "		|	indorPort: " << serverIndor.getPortUnic() << std::endl;
	serverIndor.setHost(_server.getHost());
	std::cout << "Host			: " << _server.getHost() << "	|	indorHost: " << serverIndor.getHost() << std::endl;
	serverIndor.setServerName(_server.getServerName());
	std::cout << "ServerName[0]		: " << _server.getServerName()[0] << "|	indorServerName[0]: " << serverIndor.getServerName()[0] << std::endl;
	serverIndor.setIndex(_server.getIndex());
	std::cout << "index[0]		: " << _server.getIndex()[0] << "	|	indorIndex[0]: " << serverIndor.getIndex()[0] << std::endl;
	serverIndor.setRootUnic(_server.getRoot());
	std::cout << "root			: " << _server.getRoot() << "	|	indorRoot: " << serverIndor.getRoot() << std::endl;
	serverIndor.setLocation(_server.getLocation());
	std::cout << "location		: " << _server.getLocation()[0].getPath() << "	|	indorlocation: " << serverIndor.getLocation()[0].getPath() << std::endl;

	cluster.push_back(serverIndor);
}

void	WebServer::setupCluster(void)
{
	size_t	i;
	size_t	j;

	i = 0;
	while(i < this->_nbrServers)
	{
		j = 0;
		this->_cluster[i].setupServer();
		std::cout << "Server n: " << (i + 1) << " Created in port: ";
		while (j < this->_cluster[i].getPort().size())
		{
			std::cout << this->_cluster[i].getPort(static_cast<int>(j));
			j++;
			if (j < this->_cluster[i].getPort().size())
				std::cout << ", ";
		}
		std::cout << std::endl;
		i++;
	}
}

void	WebServer::initSets(void)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < this->_nbrServers)
	{
		j = 0;
		while (j < this->_cluster[i].getSocket().size())
		{
			this->addToSet(this->_cluster[i].getSocket()[j], this->_recvSocketSet);
			this->_mapServers[this->_cluster[i].getSocket()[j++]] = this->_cluster[i];
		}
		i++;
	}
/*
*/
	i = 0;
	while (static_cast<int>(i) <= this->_biggestSocket)
	{
		if (FD_ISSET(i, &this->_recvSocketSet))
			std::cout << i << " isset" << std::endl;
		else
			std::cout << i << " isnot set" << std::endl;
		i++;
	}
	std::cout << "_biggestSocket: " << this->_biggestSocket << " size: " << this->_mapServers.size() << std::endl;
}

void	WebServer::runCluster(void)
{
	fd_set			copyRecv;
	fd_set			copyWrite;
	struct timeval	timer;
	int				i;
	while (42)
	{
		timer.tv_sec = 1;
		timer.tv_usec = 0;
		copyRecv = this->_recvSocketSet;
		copyWrite = this->_writeSocketSet;
		std::cout << "dentro do loop pré select" << std::endl;
		if (select((this->_biggestSocket + 1), &copyRecv, &copyWrite, NULL, &timer) < 0)
			continue ;
		i = 2;
		std::cout << "dentro do loop pós select" << std::endl;
		while (++i <= this->_biggestSocket)
		{
			std::cout << "socket: " << i << std::endl;
			if (FD_ISSET(i, &copyRecv))
				std::cout << "socket: " << i << " está pronto para leitura" << std::endl;

		}
	}
}
/*
*/
/*
*/

void	WebServer::addToSet(const int & port, fd_set & recvSocket)
{
	FD_SET(port, &recvSocket);
	if (port > _biggestSocket)
		this->_biggestSocket = port;
}

void	WebServer::printCluster(void) const
{
	size_t		i;
	size_t		tmp0;
	size_t		tmp1;

	i = 0;
	while (i < this->_cluster.size())
	{
		std::cout << "servidor n: " << (i + 1) << std::endl;
		tmp0 = 0;
		std::cout << "listen port: " << (this->_cluster[i].getPort())[tmp0];
		while (++tmp0 != (this->_cluster[i].getPort()).size())
			std::cout << ", "<< (this->_cluster[i].getPort())[tmp0];
		std::cout << std::endl;
		std::cout << "host: " << this->_cluster[i].getHost() << std::endl;
		tmp0 = 0;
		std::cout << "server_name: " << (this->_cluster[i].getServerName())[tmp0];
		while (++tmp0 != (this->_cluster[i].getServerName()).size())
			std::cout << ", "<< (this->_cluster[i].getServerName())[tmp0];
		std::cout << std::endl;
		tmp0 = 0;
		std::cout << "index: " << (this->_cluster[i].getIndex())[tmp0];
		while (++tmp0 != (this->_cluster[i].getIndex()).size())
			std::cout << ", " << (this->_cluster[i].getIndex())[tmp0];
		std::cout << std::endl;
		std::cout << "root: " << this->_cluster[i].getRoot() << std::endl;
		tmp0 = 0;
		std::vector<Location> indorloc = this->_cluster[i].getLocation();
		while(tmp0 < (this->_cluster[i].getLocation()).size())
		{
			std::cout << "Location n: " << (tmp0 + 1) << " {" << std::endl;
			std::cout << "Path: " << ((this->_cluster[i].getLocation())[tmp0]).getPath() << std::endl;
			tmp1 = 0;
			std::cout << "method: " << (((this->_cluster[i].getLocation())[tmp0]).getMethods())[tmp1];
			while(tmp1 < (((this->_cluster[i].getLocation())[tmp0]).getMethods()).size())
				std::cout << ", " << (((this->_cluster[i].getLocation())[tmp0]).getMethods())[tmp1++];
			std::cout << std::endl;
			std::cout << "}" << std::endl;
			tmp0++;
		}
		i++;
	}
}