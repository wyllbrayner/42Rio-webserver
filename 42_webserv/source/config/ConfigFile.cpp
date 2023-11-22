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

#include "ConfigFile.hpp"

ConfigFile::ConfigFile( void )
{
	this->_port = -1; // o range de portas validas em um sistema vai de 0 a 65535, então inicializo com -1 para saber que nada foi inserido nela.
	this->_host = 0;
}

ConfigFile	&ConfigFile::operator=( const ConfigFile &src )
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

ConfigFile::ConfigFile( const ConfigFile& copy )
{
	*this = copy;
	return ;
}

ConfigFile::~ConfigFile( void )
{
	this->_port = 0;
	this->_host = 0;
	this->_server_name.clear();
	this->_index.clear();
}


const int &	ConfigFile::getPort(void) const 
{
	return (this->_port);
}

const in_addr_t &	ConfigFile::getHost(void) const 
{
	return (this->_host);
}

const myVecS &	ConfigFile::getServerName(void) const
{
	return (this->_server_name);
}

const myVecS &	ConfigFile::getIndex(void) const
{
	return (this->_index);
}

const std::string &					ConfigFile::getRoot(void) const
{
	return (this->_root);
}


const std::vector<Location>	&		ConfigFile::getLocation(void) const
{
	return (this->_vec_location);
}

void					ConfigFile::setPort(std::string & _parameter)
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

void					ConfigFile::setHost(std::string _parameter)
{
//	std::cout << "início de setHost: " << _parameter << std::endl;
	if (this->isTokenValid(_parameter))
	{
		if (_parameter.compare(0, 9, "localhost") == 0)
			_parameter = "127.0.0.1";
		if (this->isHostValid(_parameter))
			this->_host = inet_addr(_parameter.c_str());
	}
//	std::cout << "fim    de setHost: " << _parameter << std::endl;
}

void					ConfigFile::setServerName(myItVecS &i, myVecS & sp_server)
{
//	std::cout << "início de setServerName: " << *(i) << std::endl;
	this->putVecString(i, sp_server, this->_server_name);
//	std::cout << "fim    de setServerName: " << *(i) << std::endl;
}

void					ConfigFile::setIndex(myItVecS &i, myVecS & sp_server)
{
//	std::cout << "início de setIndex: " << *(i) << std::endl;
	this->putVecString(i, sp_server, this->_index);
//	std::cout << "fim    de setIndex: " << *(i) << std::endl;
}

void					ConfigFile::setRoot(std::string & _parameter)
{
//	std::cout << "início de setRoot: " << _parameter << std::endl;
	std::string tmp;

	if (this->isTokenValid(_parameter))
	{
		if ((_parameter.size() == 1) && (_parameter.compare(0, 1, "/") == 0))
			this->_root = _parameter;
		else
		{
			if (_parameter.find("/") == std::string::npos)
			{
				tmp.append("/").append(_parameter).append("/");
				this->_root = tmp;
			}
			else
			{
				if (_parameter.find("/") != 0)
				{
					tmp.append("/").append(_parameter);
					_parameter = tmp;
				}
				if (_parameter.rfind("/") != (_parameter.size() - 1))
					_parameter.append("/");
				this->_root = _parameter;
			}
		}
	}
//	std::cout << "fim    de setRoot: " << std::endl;
}

void					ConfigFile::setLocation(myItVecS &i, myVecS & sp_server)
{
//	std::cout << "init    de setLocation: " << *i << std::endl;
	myVecS		vecLocation;
	Location	indorLocation;

	if ((*i).compare(0, 1, "{") == 0)
		throw Error::InvalidParameter();
	indorLocation.setPath(*i++);
	if ((*i++).compare(0, 1, "{") != 0)
		throw Error::InvalidParameter();
//	std::cout << "palavra: " << *i << std::endl;
	while((i != sp_server.end()) && ((*i).compare(0, 1, "}") != 0))
	{
		if (((*i).compare(0, 13, "allow_methods") == 0) || ((*i).compare(0, 7, "methods") == 0))
		{
			i++;
			while((i != sp_server.end()))
			{
				if (((*i).find(";")) != std::string::npos)
				{
					this->isTokenValid(*i);
					if (((*i).compare(0, 6, "DELETE") == 0) || ((*i).compare(0, 3, "GET") == 0) || ((*i).compare(0, 4, "POST") == 0))
						vecLocation.push_back(*i++);
					else
						throw Error::InvalidParameter();
					break ;
				}
				else
				{
					if (((*i).compare(0, 6, "DELETE") == 0) || ((*i).compare(0, 3, "GET") == 0) || ((*i).compare(0, 4, "POST") == 0))
						vecLocation.push_back(*i++);
					else
						throw Error::InvalidParameter();
				}
			}
	//		std::cout << "Location: " << *i << std::endl;
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
		this->_vec_location.push_back(indorLocation);
//	std::cout << "fim     de setLocation: " << *i << std::endl;
}
/*
void					Server::setLocation(myItVecS &i, myVecS & sp_server)
{
	std::cout << "init    de setLocation: " << *i << std::endl;
	myVecS		vecLocation;
	Location	intLocation;

	if ((*i).compare(0, 1, "{") == 0)
		throw Error::InvalidParameter();
	intLocation.setPath(*i++);
	if ((*i++).compare(0, 1, "{") != 0)
		throw Error::InvalidParameter();
//	std::cout << "palavra: " << *i << std::endl;
	if (((*i).compare(0, 13, "allow_methods") != 0) && ((*i).compare(0, 7, "methods") != 0))
		throw Error::InvalidParameter();
	else
	{
		i++;
		while((i != sp_server.end()) && ((*i).compare(0, 1, "}") != 0))
		{
			if (((*i).find(";")) != std::string::npos)
			{
				this->isTokenValid(*i);
				if (((*i).compare(0, 6, "DELETE") == 0) || ((*i).compare(0, 3, "GET") == 0) || ((*i).compare(0, 4, "POST") == 0))
					vecLocation.push_back(*i++);
				else
					throw Error::InvalidParameter();
				if ((i != sp_server.end()) && ((*i).compare(0, 1, "}") != 0))
					throw Error::InvalidParameter();
				else
					break ;
			}
			else
			{
				if (((*i).compare(0, 6, "DELETE") == 0) || ((*i).compare(0, 3, "GET") == 0) || ((*i).compare(0, 4, "POST") == 0))
					vecLocation.push_back(*i++);
				else
					throw Error::InvalidParameter();
			}
	//		std::cout << "Location: " << *i << std::endl;
		}
		if (!vecLocation.size())
			throw Error::InvalidParameter();
		intLocation.setMethods(vecLocation);
	}
	std::cout << "fim     de setLocation: " << *i << std::endl;
}
*/

bool					ConfigFile::isHostValid(std::string & _parameter)
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

bool					ConfigFile::isTokenValid( std::string & _parameter)
{
	size_t	pos;
//	std::cout << " pré _p: " << _p << std::endl;

	pos = _parameter.find(";");
	if(pos != (_parameter.size() - 1))
		throw Error::InvalidParameter();
	else
		_parameter.erase(pos);
//	std::cout << " pós _p: " << _p << std::endl;
	return (true);
}

void					ConfigFile::putVecString(myItVecS &i, myVecS & sp_server, myVecS & _vecString )
{
//	std::cout << "início de putVecString: " << *(i) << std::endl;
	std::string tmp;
//	size_t j = 0;

	while(i != sp_server.end())
	{
		tmp = *(i);
		if ((tmp.find(";")) != std::string::npos)
		{
			this->isTokenValid(tmp);
			_vecString.push_back(tmp);
//			std::cout << "putVecString: " << tmp << std::endl;
//			std::cout << "VecString: " ;
//			while (j < _vecString.size())
//			{
//				std::cout << _vecString[j] << " ";
//				j++;
//			}
//			std::cout << std::endl;
			break ;
		}
		else
		{
			_vecString.push_back(tmp);
//			std::cout << "putVecString: " << tmp << std::endl;
		}
		i++;
	}
//	std::cout << "fim    de putVecString: " << std::endl;
	tmp.clear();
}
