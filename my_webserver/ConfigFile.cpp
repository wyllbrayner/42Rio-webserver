/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:23:54 by woliveir          #+#    #+#             */
/*   Updated: 2022/05/09 15:09:49 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigFile.hpp"

ConfigFile::ConfigFile(void)
{
	this->_host = 0;
}

ConfigFile	&ConfigFile::operator=(const ConfigFile &src)
{
	if (this != &src)
	{
		this->_port = src.getPort();
		this->_host = src.getHost();
		this->_server_name = src.getServerName();
		this->_index = src.getIndex();
		this->_root = src.getRoot();
		this->_vec_location = src.getLocation();
	}
	return (*this);
}

ConfigFile::ConfigFile(const ConfigFile& copy)
{
	*this = copy;
	return ;
}

ConfigFile::~ConfigFile(void)
{
	this->_host = 0;
}

const std::vector<int> &	ConfigFile::getPort(void) const 
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

const std::vector<Location>		& ConfigFile::getLocation(void) const
{
	return (this->_vec_location);
}

void					ConfigFile::setPort(myItVecS &i, myVecS & sp_server)
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

void					ConfigFile::setHost(std::string _parameter)
{
	if (this->isTokenValid(_parameter))
	{
		if (_parameter.compare(0, 9, "localhost") == 0)
			_parameter = "127.0.0.1";
		if (this->isHostValid(_parameter))
			this->_host = inet_addr(_parameter.c_str());
	}
}

void					ConfigFile::setServerName(myItVecS &i, myVecS & sp_server)
{
	this->putVecString(i, sp_server, this->_server_name);
}

void					ConfigFile::setServerNameSmart(std::string _parameter)
{
	if (this->isTokenValid(_parameter))
		this->_server_name.push_back(_parameter);
}

void					ConfigFile::setIndex(myItVecS &i, myVecS & sp_server)
{
	this->putVecString(i, sp_server, this->_index);
}

void					ConfigFile::setIndexSmart(std::string _parameter)
{
	if (this->isTokenValid(_parameter))
		this->_index.push_back(_parameter);
}

void					ConfigFile::setRoot(std::string _parameter)
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

void					ConfigFile::setLocation(myItVecS &i, myVecS & sp_server)
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
		this->_vec_location.push_back(indorLocation);
}

bool					ConfigFile::isTokenValid( std::string & _parameter)
{
	size_t	pos;

	pos = _parameter.find(";");
	if(pos != (_parameter.size() - 1))
		throw Error::InvalidParameter();
	else
		_parameter.erase(pos);
	return (true);
}

bool					ConfigFile::isHostValid(std::string & _parameter)
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

void					ConfigFile::putVecString(myItVecS &i, myVecS & sp_server, myVecS & _vecString )
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

void					ConfigFile::putVecInt(std::string & _parameter)
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

bool					ConfigFile::portIsDuplic(void) const
{
	std::set<int>	tmp;

	if (this->_port.size() == 1)
		return (false);
	tmp.insert(this->_port.begin(), this->_port.end());
	if (this->_port.size() != tmp.size())
		return (true);
	return (false);
}