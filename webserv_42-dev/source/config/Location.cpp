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

Location::Location(void) {}

Location	&Location::operator=(const Location & src)
{
	if (this != &src)
	{
		this->_path = src._path;
		this->_allowed_methods = src._allowed_methods;
	}
	return (*this);
}

Location::Location(const Location & copy)
{
	*this = copy;
	return ;
}

Location::~Location(void) {}

const std::string &					Location::getPath(void) const
{
	return (this->_path);
}

const std::vector<std::string>		Location::getMethods(void) const
{
	return (this->_allowed_methods);
}

void								Location::setPath(std::string & path)
{
//	std::cout << "int     de setPath: " << path << std::endl;
	this->_path = path;
//	std::cout << "end     de setPath: " << this->_path << std::endl;
}

void								Location::setMethods(std::vector<std::string> & vecLocation)
{
//	std::cout << "int     de setMethods: " << vecLocation[0] << std::endl;
	this->_allowed_methods = vecLocation;
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

//	std::cout << "end     de setMethods: " << vecLocation[0] << std::endl;
}