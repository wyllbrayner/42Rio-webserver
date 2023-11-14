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
		this->_path = src.getPath();
		this->_allowedMethods = src.getMethods();
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
	return (this->_allowedMethods);
}

void								Location::setPath(std::string & path)
{
	this->_path = path;
}

void								Location::setMethods(std::vector<std::string> & vecLocation)
{
	this->_allowedMethods = vecLocation;
}