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

Server::Server( void )
{
	this->_port = -1; // o range de portas validas em um sistema vai de 0 a 65535, então inicializo com -1 para saber que nada foi inserido nela.
	this->_host = 0;
}

Server	&Server::operator=( const Server &src )
{
	if (this != &src)
	{
		this->_port = src.getPort();
		this->_host = src.getHost();
		this->_server_name = src.getServerName();
		this->_index = src.getIndex();
		this->_root = src.getRoot();
	}
	return (*this);
}

Server::Server( const Server& copy )
{
	*this = copy;
	return ;
}

Server::~Server( void )
{
	this->_port = 0;
	this->_host = 0;
	this->_server_name.clear();
	this->_index.clear();
}


const int &	Server::getPort(void) const 
{
	return (this->_port);
}

const in_addr_t &	Server::getHost(void) const 
{
	return (this->_host);
}

const std::vector<std::string> &	Server::getServerName(void) const
{
	return (this->_server_name);
}

const std::string &					Server::getIndex(void) const
{
	return (this->_index);
}

const std::string &					Server::getRoot(void) const
{
	return (this->_root);
}

void					Server::setPort(std::string & _parameter)
{
	int 				nbr_port;
	unsigned short int	i;
	
	nbr_port = 0;
	i = 0;
	if (this->isTokenValid(_parameter))
	{
		while (i < _parameter.size())
		{
			if ( !std::isdigit(_parameter[i]))
				throw Error::InvalidParameter();
			i++;
		}
		if ( i > 5 ) // 0 maior valor para uma porta válida é 65535, logo possui, no máximo, 5 dígitos.
			throw Error::InvalidParameter();
		nbr_port = Utils::atoi(_parameter);
		if (nbr_port > 65535 || nbr_port < 0)
			throw Error::InvalidParameter();
		else
			this->_port = nbr_port;
		nbr_port = 0;
	}
}

void					Server::setHost(std::string & _parameter)
{
//	std::cout << "início de setHost: " << _parameter << std::endl;
	if (this->isTokenValid(_parameter))
	{
		if (_parameter.compare(0, 9, "localhost" ) == 0)
			_parameter = "127.0.0.1";
		if (this->isHostValid(_parameter ))
			this->_host = inet_addr(_parameter.c_str());
	}
//	std::cout << "fim    de setHost: " << _parameter << std::endl;
}

void					Server::setServerName(std::vector<std::string>::iterator &i, std::vector<std::string> & sp_server)
{
//	std::cout << "início de setServerName: " << *(i) << std::endl;
	std::string tmp;
//	size_t j = 0;

	while( i != sp_server.end())
	{
		tmp = *(i);
		if ((tmp.find(";")) != std::string::npos)
		{
			this->isTokenValid(tmp);
			this->_server_name.push_back(tmp);
//			std::cout << "server_name: " << tmp << std::endl;
/*
			std::cout << "setServerName: " ;
			while (j < this->_server_name.size())
			{
				std::cout << this->_server_name[j] << " ";
				j++;
			}
			std::cout << std::endl;
*/
			break ;
		}
		else
		{
			this->_server_name.push_back(tmp);
//			std::cout << "server_name: " << tmp << std::endl;
		}
		i++;
	}
//	std::cout << "fim    de setServerName: " << std::endl;
	tmp.clear();
}

void					Server::setIndex(std::string & _parameter)
{
//	std::cout << "início de setIndex: " << _parameter << std::endl;
	if (this->isTokenValid(_parameter))
		this->_index = _parameter;
//	std::cout << "fim    de setIndex: " << _parameter << std::endl;
}

void					Server::setRoot(std::string & _parameter)
{
//	std::cout << "início de setRoot: " << _parameter << std::endl;
	char path[4096];
	std::string fullPath;
	DIR *directory;

	directory = NULL;
	if (this->isTokenValid(_parameter))
	{
		if (getcwd(path, 4096))
		{
			fullPath = path;
			fullPath.append("/").append(_parameter);
			directory = opendir(fullPath.c_str());
			if (!directory)
				throw Error::InvalidParameter();
			else
			{
				this->_root = fullPath;
				closedir(directory);
			}
		}
		else
			throw Error::InvalidParameter();
	}
//	std::cout << "fim    de setRoot: " << _parameter << std::endl;
}

bool					Server::isHostValid(std::string & _parameter)
{
//	std::cout << "início de isHostValid: " << _parameter << std::endl;
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
//		std::cout << "nbr: " << nbr << std::endl;
		i += (j + 1);
	}
//	std::cout << "fim    de isHostValid: " << _parameter << std::endl;
	return (true);
}

bool					Server::isTokenValid( std::string & _parameter)
{
	size_t	pos;
//	std::cout << " pré _p: " << _p << std::endl;

	pos = _parameter.find(";");
	if( pos != (_parameter.size() - 1))
		throw Error::InvalidParameter();
	else
		_parameter.erase(pos);
//	std::cout << " pós _p: " << _p << std::endl;
	return (true);
}