/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:23:54 by woliveir          #+#    #+#             */
/*   Updated: 2022/05/09 15:09:49 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Error.hpp"

const char *	Error::InvalidArg::what( void ) const throw()
{
	return ("Error:Invalid arguments.\nPlease enter webserver <config file>");
}

const char *	Error::InvalidPathServer::what( void ) const throw()
{
	return ("Error: could not open config file.");
}

const char *	Error::InvalidConfigurationServer::what( void ) const throw()
{
	return ("Error: server has configuration errors");
}

const char *	Error::InvalidServer::what( void ) const throw()
{
	return ("Error: server did not find.");
}

const char *	Error::InvalidParameter::what( void ) const throw()
{
	return ("Error: Invalid parameter.");
}
