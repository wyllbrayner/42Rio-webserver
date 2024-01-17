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

ConfigFile::ConfigFile(void) : _serverDefault(false), _host(0), \
_maxBodySize(-1) {}

ConfigFile					&	ConfigFile::operator=(const ConfigFile & src)
{
	if (this != &src)
	{
		this->_serverDefault = src.getIsServerDefault();
		this->_port = src.getPort();
		this->_root = src.getRoot();
		this->_serverName = src.getServerName();
		this->_host = src.getHost();
		this->_index = src.getIndex();
		this->_maxBodySize = src.getMaxBodySize();
		this->_errorPage = src.getErrorPage();
		this->_vecLocation = src.getLocation();
	}
	return (*this);
}

ConfigFile::ConfigFile(const ConfigFile & copy)
{
	*this = copy;
	return ;
}

ConfigFile::~ConfigFile(void)
{
	this->clearConfFile();
}

const bool					&	ConfigFile::getIsServerDefault(void) const
{
	return (this->_serverDefault);
}

const std::vector<int>		&	ConfigFile::getPort(void) const
{
	return (this->_port);
}

const in_addr_t				&	ConfigFile::getHost(void) const
{
	return (this->_host);
}

const myVecS				&	ConfigFile::getServerName(void) const
{
	return (this->_serverName);
}

const myVecS				&	ConfigFile::getIndex(void) const
{
	return (this->_index);
}

const std::string			&	ConfigFile::getRoot(void) const
{
	return (this->_root);
}

const int					&	ConfigFile::getMaxBodySize(void) const
{
	return (this->_maxBodySize);
}

const myVecS				&	ConfigFile::getErrorPage(void) const
{
	return (this->_errorPage);
}

const std::vector<Location>	&	ConfigFile::getLocation(void) const
{
	return (this->_vecLocation);
}

void							ConfigFile::setServerDefaultSmart(bool _parameter)
{
	this->_serverDefault = _parameter;
}

void							ConfigFile::setPort(myItVecS & i, myVecS & sp_server)
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

void							ConfigFile::setPortSmart(std::string _parameter)
{
	this->isTokenValid(_parameter);
	putVecInt(_parameter);
}

void							ConfigFile::setPort(int _parameter)
{
	this->_port.push_back(_parameter);
}

void							ConfigFile::setHost(std::string _parameter)
{
	if (this->isTokenValid(_parameter))
	{
		if (_parameter.compare(0, 9, "localhost") == 0)
			_parameter = "127.0.0.1";
		if (this->isHostValid(_parameter))
			this->_host = inet_addr(_parameter.c_str());
	}
}

void							ConfigFile::setHost(const in_addr_t & _parameter)
{
	this->_host = _parameter;
}

void							ConfigFile::setServerName(myItVecS & i, myVecS & sp_server)
{
	this->putVecString(i, sp_server, this->_serverName);
}

void							ConfigFile::setServerNameSmart(std::string _parameter)
{
	if (this->isTokenValid(_parameter))
		this->_serverName.push_back(_parameter);
}

void							ConfigFile::setServerName(const myVecS & _parameter)
{
	this->_serverName = _parameter;
}

void							ConfigFile::setIndex(myItVecS & i, myVecS & sp_server)
{
	this->putVecString(i, sp_server, this->_index);
}

void							ConfigFile::setIndexSmart(std::string _parameter)
{
	this->isTokenValid(_parameter);
	this->_index.push_back(_parameter);
}

void							ConfigFile::setIndex(const myVecS & _parameter)
{
	this->_index = _parameter;
}

void							ConfigFile::setRoot(std::string _parameter)
{
	std::string	tmp;
	std::string	root;

	root = "/var/www/html";
	if (this->isTokenValid(_parameter))
	{
		if (((_parameter.compare(0, root.size(), root) == 0) && \
			(_parameter.size() == root.size())) || ((_parameter.size() == 1) && \
			(_parameter.compare(0, 1, "/") == 0)) || _parameter.empty())
			this->_root = root.append("/");
		else
		{
			if ((_parameter.compare(0, root.size(), root)) == 0 && \
									(_parameter.size() > root.size()))
			{
				if (_parameter.rfind("/") != (_parameter.size() - 1))
					this->_root = _parameter.append("/");
				else
					this->_root = _parameter;
			}
			else
			{
				if (_parameter.rfind("/") != (_parameter.size() - 1))
					_parameter.append("/");
				if (_parameter.find("/") != 0)
					this->_root = root.append("/").append(_parameter);
				else
					this->_root = root.append(_parameter);
			}
		}
	}
}

void							ConfigFile::setRootSmart(const std::string & _parameter)
{
	this->_root = _parameter;
}

void							ConfigFile::setMaxBodySize(std::string _parameter)
{
	unsigned short int	i;

	i = 0;
	this->isTokenValid(_parameter);
	while (i < _parameter.size())
	{
		if (!std::isdigit(_parameter[i]))
			throw Error::InvalidParameter();
		i++;
	}
	this->_maxBodySize = Utils::atoi(_parameter);
	i = 0;
}

void							ConfigFile::setMaxBodySize(const int & _parameter)
{
	this->_maxBodySize = _parameter;
}

void							ConfigFile::setErrorPage(myItVecS & i, myVecS & sp_server)
{
	this->putVecString(i, sp_server, this->_errorPage);
}

void							ConfigFile::setErrorPageSmart(std::string _parameter)
{
	if (this->isTokenValid(_parameter))
		this->_errorPage.push_back(_parameter);
}

void							ConfigFile::setErrorPage(const myVecS & _parameter)
{
	this->_errorPage = _parameter;
}

void							ConfigFile::setLocation(myItVecS & i, \
			myVecS & sp_server, const std::string & root, const std::string & index)
{
	myVecS		vecIndex;
	myVecS		vecMethod;
	Location	indorLocation;

	if ((*i).compare(0, 1, "{") == 0)
		throw Error::InvalidParameter();
	indorLocation.setPath(*i++, root);
	if ((*i++).compare(0, 1, "{") != 0)
		throw Error::InvalidParameter();
	while((i != sp_server.end()) && ((*i).compare(0, 1, "}") != 0))
	{
		if (((*i).compare(0, 5, "index") == 0))
		{
			i++;
			while((i != sp_server.end()) && ((*i).compare(0, 1, "}") != 0) && \
				(((*i).find(";")) == std::string::npos))
				vecIndex.push_back(*i++);
			if (((*i).find(";")) != std::string::npos)
			{
				this->isTokenValid(*i);
				vecIndex.push_back(*i++);
			}
			indorLocation.setIndex(vecIndex);
		}
		else if (((*i).compare(0, 13, "allow_methods") == 0) || ((*i).compare(0, 7, "methods") == 0))
		{
			i++;
			while((i != sp_server.end()) && ((*i).compare(0, 1, "}") != 0))
			{
				if (((*i).find(";")) != std::string::npos)
				{
					this->isTokenValid(*i);
					if (find(indorLocation.getAllowedMethods().begin(), indorLocation.getAllowedMethods().end(), *i) != indorLocation.getAllowedMethods().end())
						vecMethod.push_back(*i++);
					else
						throw Error::InvalidParameter();
					break ;
				}
				else
				{
					if (find(indorLocation.getAllowedMethods().begin(), indorLocation.getAllowedMethods().end(), *i) != indorLocation.getAllowedMethods().end())
						vecMethod.push_back(*i++);
					else
						throw Error::InvalidParameter();
				}
			}
			if (!vecMethod.size())
				throw Error::InvalidParameter();
			indorLocation.setMethods(vecMethod);
		}
		else if (((*i).compare(0, 9, "autoindex") == 0))
		{
			i++;
			if (i != sp_server.end() && (((*i).find(";")) != std::string::npos))
			{
				this->isTokenValid(*i);
				if ((*i).compare(0, 2, "on") == 0)
					indorLocation.setAutoIndex(true);
				else if ((*i).compare(0, 3, "off") == 0)
					indorLocation.setAutoIndex(false);
				else
					throw Error::InvalidParameter();
				i++;
			}
			else
				throw Error::InvalidParameter();
		}
		else if (((*i).compare(0, 6, "return") == 0))
		{
			i++;
			if (i != sp_server.end() && (((*i).find(";")) != std::string::npos))
			{
				this->isTokenValid(*i);
				indorLocation.setReturn(*i, root);
			}
			else
				throw Error::InvalidParameter();
		}
		else
		{
			if (i != sp_server.end())
				i++;
		}
	}
	indorLocation.fixeReturn();
	if (indorLocation.getMethods().empty())
		indorLocation.setDefaultMethods();
	if (indorLocation.getIndex().empty())
		indorLocation.setIndexSmart(index);
	this->_vecLocation.push_back(indorLocation);
}

void							ConfigFile::setLocation(const std::vector<Location> & _parameter)
{
	this->_vecLocation = _parameter;
}

bool							ConfigFile::isHostValid(std::string & _parameter)
{
	int					nbr;
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

bool							ConfigFile::isTokenValid(std::string & _parameter)
{
	size_t	pos;

	pos = _parameter.find(";");
	if (pos != (_parameter.size() - 1) && (_parameter.find(";}") != (_parameter.size() - 2)))
		throw Error::InvalidParameter();
	else
	{
		if (_parameter.find(";}") == (_parameter.size() - 2))
			_parameter.erase(pos, 2);
		else
			_parameter.erase(pos);
	}
	return (true);
}

void							ConfigFile::putVecString(myItVecS & i, myVecS & sp_server, myVecS & _vecString)
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
	tmp.clear();
}

void							ConfigFile::putVecInt(std::string & _parameter)
{
	int					nbr_port;
	unsigned short int	i;

	nbr_port = 0;
	i = 0;
	if (_parameter.size() > 5)
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

bool							ConfigFile::portIsDuplic(void) const
{
	std::set<int>	tmp;

	if (this->_port.size() == 1)
		return (false);
	tmp.insert(this->_port.begin(), this->_port.end());
	if (this->_port.size() != tmp.size())
		return (true);
	return (false);
}

void							ConfigFile::fixeErrorPage(void)
{
	size_t						i;
	std::string					root;
	std::vector<std::string>	tmp00;
	std::vector<std::string>	tmp01;

	tmp00 = this->getErrorPage();
	if (tmp00.empty())
		this->setErrorPageSmart("default;");
	else
	{
		root = this->getRoot();
		i = 0;
		while (i < tmp00.size())
		{
			if ((tmp00[i].compare(0, root.size(), root)) == 0)
				tmp01.push_back(tmp00[i]);
			else
			{
				if (tmp00[i].find("/") == 0)
					tmp00[i].erase(0, 1);
				tmp01.push_back(root.append(tmp00[i]));
				root = this->getRoot();
			}
			i++;
		}
		this->_errorPage.clear();
		i = 0;
		while (i < tmp01.size())
			this->_errorPage.push_back(tmp01[i++]);
	}
}

void							ConfigFile::findLastReturn(void)
{
	size_t		i;
	ssize_t		Loc;
	std::string	tmp;

	i = 0;
	while (i < this->getLocation().size())
	{
		if (this->getLocation()[i].getPath() != this->getLocation()[i].getReturn())
		{
			Loc = findReturn(this->getLocation()[i].getReturn(), i, 0);
			if (Loc != -1)
			{
				tmp = this->getLocation()[i].getPath();
				this->_vecLocation[i] = this->getLocation()[Loc];
				this->_vecLocation[i].setPathSmart(tmp);
			}
			else
				this->_vecLocation[i].setReturnSmart("Error500");
		}
		i++;
	}
}

ssize_t							ConfigFile::findReturn(const std::string & _return, const size_t & i, size_t limit)
{
	size_t	j;

	j = 0;
	while (j < this->getLocation().size() && limit < 1024)
	{
		if (j != i)
		{
			if (this->getLocation()[j].getPath() == _return)
			{
				if (this->getLocation()[j].getPath() != this->getLocation()[j].getReturn())
					return (findReturn(this->getLocation()[j].getReturn(), j, (limit + 1)));
				else
					return (j);
			}
		}
		j++;
	}
	return (-1);
}

void							ConfigFile::clearConfFile(void)
{
	size_t		i;
	Location	loc;

	i = 0;
	this->_port.clear();
	this->_root.clear();
	this->_serverName.clear();
	this->_host = 0;
	this->_index.clear();
	this->_maxBodySize = -1;
	this->_errorPage.clear();
	while (i < this->getLocation().size())
	{
		loc = this->getLocation()[i];
		loc.clearLocation();
		i++;
	}
	this->_vecLocation.clear();
}

void							ConfigFile::printConfigFile(void) const
{
	size_t	i;

	if (this->getIsServerDefault())
		std::cout << "Type\t\t: Default" << std::endl;
	else
		std::cout << "Type\t\t: Not Default" << std::endl;
	this->printVec(this->getPort(), "port\t\t: ");
	std::cout << "root\t\t: " << this->getRoot() << std::endl;
	this->printVec(this->getServerName(), "Server Name\t: ");
	std::cout << "host\t\t: " << this->getHost() << std::endl;
	this->printVec(this->getIndex(), "Index\t\t: ");
	std::cout << "Max Body Size\t: " << this->getMaxBodySize() << std::endl;
	this->printVec(this->getErrorPage(), "Error Page\t: ");
	if (this->getLocation().size())
	{
		i = 0;
		while (i < this->getLocation().size())
		{
			std::cout << "Location n\t: " << (i + 1) << std::endl;
			this->getLocation()[i++].printLocation();
		}
	}
}