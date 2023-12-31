/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:23:54 by woliveir          #+#    #+#             */
/*   Updated: 2022/05/09 15:09:49 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location(void)
{
	this->_allowedMethods.push_back("GET");
	this->_allowedMethods.push_back("DELETE");
	this->_allowedMethods.push_back("POST");
	this->_autoIndex = false;
}

Location						&	Location::operator=(const Location & src)
{
	if (this != &src)
	{
		this->_path = src.getPath();
		this->_methods = src.getMethods();
		this->_index = src.getIndex();
		this->_autoIndex = src.getAutoIndex();
		this->_return = src.getReturn();
	}
	return (*this);
}

Location::Location(const Location & copy)
{
	*this = copy;
	return ;
}

Location::~Location(void)
{
	this->clearLocation();
}

const std::vector<std::string>	&	Location::getAllowedMethods(void) const
{
	return (this->_allowedMethods);
}

const std::string				&	Location::getPath(void) const
{
	return (this->_path);
}

const std::vector<std::string>	&	Location::getMethods(void) const
{
	return (this->_methods);
}

const std::vector<std::string>	&	Location::getIndex(void) const
{
	return (this->_index);
}

const bool						&	Location::getAutoIndex(void) const
{
	return (this->_autoIndex);
}

const std::string				&	Location::getReturn(void) const
{
	return (this->_return);
}

void								Location::setPath(std::string & path, std::string root)
{
//	std::cout << "int     de setPath: " << path << " com Root: " << root << std::endl;
	this->_path = root.append(Utils::setPlace(path));
//	std::cout << "end     de setPath: " << this->_path << std::endl;
}

void								Location::setPathSmart(std::string & path)
{
	this->_path = path;
}

void								Location::setMethods(\
									std::vector<std::string> & vecMethods)
{
//	std::cout << "int     de setMethods: " << vecMethods[0] << std::endl;
	this->_methods = vecMethods;
/*
	size_t j = 0;
//	std::cout << "setMethods: ";
	while(j < this->_allowed_methods.size())
	{
		std::cout << this->_allowed_methods[j] << " ";
		j++;
	}
	std::cout << std::endl;
*/
//	std::cout << "end     de setMethods: " << vecMethods[0] << std::endl;
}

void								Location::setDefaultMethods(void)
{
//	std::cout << "int     de setMethods: " << vecMethods[0] << std::endl;
	this->_methods.push_back("GET");
	this->_methods.push_back("DELETE");
	this->_methods.push_back("POST");
//	std::cout << "end     de setMethods: " << vecMethods[0] << std::endl;
}

void								Location::setIndex(\
									std::vector<std::string> & vecIndex)
{
	this->_index = vecIndex;
}

void								Location::setIndexSmart(std::string parameter)
{
	this->_index.push_back(parameter);
}

void								Location::setAutoIndex(bool flag)
{
	this->_autoIndex = flag;
}

void								Location::setReturn(std::string path, std::string root)
{
//	std::cout << "int     de setReturn: " << path << " com Root: " << root << std::endl;
	this->_return = root.append(Utils::setPlace(path));
//	std::cout << "end     de setReturn: " << this->_return << std::endl;
}

void								Location::setReturnSmart(std::string path)
{
	this->_return = path;
}
/*
const std::string				&	Location::setPlace(std::string & path)
{
//	std::cout << "int     de setPlace: " << path << std::endl;
	if (path.find("/") == 0)
		path.erase(0, 1);
	if (path.rfind("/") != (path.size() - 1))
		path.append("/");
//	std::cout << "end     de setPlace: " << path << std::endl;
	return (path);
}
*/

void								Location::fixeReturn(void)
{
	if (this->getReturn().empty())
		this->_return = this->getPath();
}

void								Location::printLocation(void) const
{
//	std::cout << "init printLocation" << std::endl;
	std::cout << "  => path\t: " << this->getPath() << std::endl;
	this->printVecString(this->getMethods(), "  => methods\t: ");
	this->printVecString(this->getIndex(), "  => index\t: ");
	if (this->getAutoIndex())
		std::cout << "  => autoIndex\t: " << "true" << std::endl;
	else
		std::cout << "  => autoIndex\t: " << "false" << std::endl;
	std::cout << "  => return\t: " << this->getReturn() << std::endl;
//	std::cout << "fim  printLocation" << std::endl;
}

void								Location::printVecString(\
			const std::vector<std::string> & content, std::string str) const
{
	size_t	i;

	i = 0;
	std::cout << str;
	while (i < content.size())
	{
		std::cout << content[i];
		if ((i + 1) < content.size())
			std::cout << ", ";
		i++;
	}
	std::cout << std::endl;
}

void								Location::clearLocation(void)
{
	this->_path.clear();
	this->_methods.clear();
	this->_index.clear();
	this->_autoIndex = false;
	this->_return.clear();
}