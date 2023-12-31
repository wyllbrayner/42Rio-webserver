/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:23:54 by woliveir          #+#    #+#             */
/*   Updated: 2022/05/09 15:09:49 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(void)
{
	this->_host = 0;
}

Server	&Server::operator=(const Server &src)
{
	if (this != &src)
	{
		this->_port = src.getPort();
		this->_host = src.getHost();
		this->_serverName = src.getServerName();
		this->_index = src.getIndex();
		this->_root = src.getRoot();
		this->_vecLocation = src.getLocation();
		this->_socket = src.getSocket();
	}
	return (*this);
}

Server::Server(const Server& copy)
{
	*this = copy;
	return ;
}

Server::~Server(void)
{
	this->_host = 0;
}

const std::vector<int>	&	Server::getPort(void) const 
{
	return (this->_port);
}

const int				&	Server::getPortUnic(void) const 
{
	return (this->getPort()[0]);
}

const int				&	Server::getPort(const size_t & _port) const
{
	return (this->getPort()[_port]);
}

const in_addr_t &	Server::getHost(void) const 
{
	return (this->_host);
}

const myVecS &	Server::getServerName(void) const
{
	return (this->_serverName);
}

const std::string &	Server::getServerName(const size_t & _i) const
{
	return (this->getServerName()[_i]);
}

const std::string &	Server::getServerNameUnic(void) const
{
	return (this->getServerName()[0]);
}

const myVecS &	Server::getIndex(void) const
{
	return (this->_index);
}

const std::string &	Server::getIndex(const size_t & _i) const
{
	return (this->getIndex()[_i]);
}

const std::string &	Server::getIndexUnic(void) const
{
	return (this->getIndex()[0]);
}

const std::string &					Server::getRoot(void) const
{
	return (this->_root);
}

const std::vector<Location>		&	Server::getLocation(void) const
{
	return (this->_vecLocation);
}

const Location 					&	Server::getLocation(const size_t & _i) const
{
	return (this->_vecLocation[_i]);
}

const std::vector<int> &		Server::getSocket(void) const
{
	return (this->_socket);
}

const std::vector<struct sockaddr_in> &		Server::getServerAddress(void) const
{
	return (this->_serverAddress);
}

void					Server::setPort(myItVecS &i, myVecS & sp_server)
{
	myVecS	tmp;
	size_t	j;

	j = 0;
	this->putVecString(i, sp_server, tmp);
	while (j < tmp.size())
		putVecInt(tmp[j++]);
	if (portIsDuplic())
		throw Error::InvalidParameter();
}

void					Server::setPort(const int & _port)
{
	this->_port.push_back(_port);
}

void					Server::setHost(std::string _parameter)
{
	if (this->isTokenValid(_parameter))
	{
		if (_parameter.compare(0, 9, "localhost") == 0)
			_parameter = "127.0.0.1";
		if (this->isHostValid(_parameter))
			this->_host = inet_addr(_parameter.c_str());
	}
}

void					Server::setHost(const in_addr_t & host)
{
	this->_host = host;
}

void					Server::setServerName(const myVecS _sn)
{
	this->_serverName = _sn;
}

void					Server::setServerName(myItVecS &i, myVecS & sp_server)
{
	this->putVecString(i, sp_server, this->_serverName);
}

void					Server::setServerNameSmart(std::string _parameter)
{
	if (this->isTokenValid(_parameter))
		this->_serverName.push_back(_parameter);
}

void					Server::setIndex(const myVecS & _idx)
{
	this->_index = _idx;
}

void					Server::setIndex(myItVecS &i, myVecS & sp_server)
{
	this->putVecString(i, sp_server, this->_index);
}

void					Server::setIndexSmart(std::string _parameter)
{
	if (this->isTokenValid(_parameter))
		this->_index.push_back(_parameter);
}

void					Server::setRootUnic(const std::string & _rt)
{
	this->_root = _rt;
}

void					Server::setRoot(std::string _parameter)
{
	if (this->isTokenValid(_parameter))
	{
		if (_parameter.compare(0, 1, "/") != 0)
		{
			if ((_parameter.rfind("/") + 1) != _parameter.size())
				_parameter.append("/");
		}
		this->_root = _parameter;
	}
}

void					Server::setLocation(myItVecS &i, myVecS & sp_server)
{
	myVecS		vecLocation;
	Location	indorLocation;

	if ((*i).compare(0, 1, "{") == 0)
		throw Error::InvalidParameter();
	indorLocation.setPath(*i++);
	if ((*i++).compare(0, 1, "{") != 0)
		throw Error::InvalidParameter();
	while((i != sp_server.end()) && ((*i).compare(0, 1, "}") != 0))
	{
		if (((*i).compare(0, 13, "allow_methods") == 0) || ((*i).compare(0, 7, "methods") == 0))
		{
			i++;
			while((i != sp_server.end()))
			{
				if (((*i).compare(0, 6, "DELETE") == 0) || ((*i).compare(0, 3, "GET") == 0) || ((*i).compare(0, 4, "POST") == 0))
				{
					if (((*i).find(";")) != std::string::npos)
					{
						this->isTokenValid(*i);
						vecLocation.push_back(*i++);
						break ;
					}
					else
						vecLocation.push_back(*i++);
				}
				else
						throw Error::InvalidParameter();
			}
			if (!vecLocation.size())
				throw Error::InvalidParameter();
			indorLocation.setMethods(vecLocation);
		}
		else
		{
			while((i != sp_server.end()) && ((*i).compare(0, 1, "}") != 0))
				i++;
		}
	}
	if (vecLocation.size())
		this->_vecLocation.push_back(indorLocation);
}

void					Server::setLocation(const std::vector<Location> & _loc)
{
	this->_vecLocation = _loc;
}

bool					Server::isTokenValid( std::string & _parameter)
{
	size_t	pos;

	pos = _parameter.find(";");
	if(pos != (_parameter.size() - 1))
		throw Error::InvalidParameter();
	else
		_parameter.erase(pos);
	return (true);
}

bool					Server::isHostValid(std::string & _parameter)
{
	int 				nbr;
	unsigned short int	i;
	unsigned short int	j;
	
	i = 0;
	if ((_parameter.size() > 15) || (_parameter.size() < 7))
		throw Error::InvalidParameter();
	while (i < _parameter.size())
	{
		j = 0;
		while (j < 3 && ((i + j) < _parameter.size()) && \
						(_parameter[(i + j)] != '.'))
		{
			if (!std::isdigit(_parameter[(i + j)]))
				throw Error::InvalidParameter();
			j++;
		}
		if ((_parameter[(i + j)] != '.') && ((i + j) < _parameter.size()))
			throw Error::InvalidParameter();
		nbr = Utils::atoi(_parameter.substr(i, j));
		if ((nbr > 255) || (nbr < 0))
			throw Error::InvalidParameter();
		i += (j + 1);
	}
	return (true);
}

void					Server::putVecString(myItVecS &i, myVecS & sp_server, myVecS & _vecString )
{
	std::string	tmp;

	while(i != sp_server.end())
	{
		tmp = *(i);
		if ((tmp.find(";")) != std::string::npos)
		{
			this->isTokenValid(tmp);
			_vecString.push_back(tmp);
			break ;
		}
		else
			_vecString.push_back(tmp);
		i++;
	}
}

void					Server::putVecInt(std::string & _parameter)
{
	int 				nbr_port;
	unsigned short int	i;
	
	nbr_port = 0;
	i = 0;
	if (_parameter.size() > 5) // 0 maior valor para uma porta válida é 65535, logo possui, no máximo, 5 dígitos.
		throw Error::InvalidParameter();
	while (i < _parameter.size())
	{
		if (!std::isdigit(_parameter[i]))
			throw Error::InvalidParameter();
		i++;
	}
	nbr_port = Utils::atoi(_parameter);
	if (nbr_port > 65535 || nbr_port <= 0)
		throw Error::InvalidParameter();
	else
		this->_port.push_back(nbr_port);
	nbr_port = 0;
	i = 0;
}

bool					Server::portIsDuplic(void) const
{
	std::set<int>	tmp;

	if (this->_port.size() == 1)
		return (false);
	tmp.insert(this->_port.begin(), this->_port.end());
	if (this->_port.size() != tmp.size())
		return (true);
	return (false);
}

void					Server::setupServer(void)
{
	size_t	i;
	int		tmpSocket;
	int		value;

	i = 0;
	value = 1;
	while (i < this->_port.size())
	{
		tmpSocket = socket(AF_INET, SOCK_STREAM, 0); // cria um socket associando o IPV4 ao TCP). 
		if (tmpSocket == -1)
			throw Error::InvalidSocket();
		this->_socket.push_back(tmpSocket);
	    setsockopt(this->_socket[i], SOL_SOCKET, SO_REUSEADDR, &value, \
					sizeof(int)); //useful for quickly reusing a port in case of server failure. 
		struct sockaddr_in	tmpAddrIn;
		bzero(&tmpAddrIn, sizeof(tmpAddrIn));
	    tmpAddrIn.sin_family = AF_INET;
	    tmpAddrIn.sin_addr.s_addr = this->getHost();
    	tmpAddrIn.sin_port = htons(this->getPort(i));
		this->_serverAddress.push_back(tmpAddrIn);
	    if (bind(this->_socket[i], (struct sockaddr *) &this->_serverAddress[i], \
					sizeof(this->_serverAddress[i])) == -1) //associa o socket à port e host do servidor.
			throw Error::ImpossibleToBind();
		if (listen(this->_socket[i], 1024) < 0) //determina que o socket ficará escutando até 1024 chamadas pendentes.
			throw Error::ImpossibleToListen();
        if (fcntl(this->_socket[i], F_SETFL, O_NONBLOCK) < 0)
			throw Error::ImpossibleToNonblock();
		i++;
	}
}