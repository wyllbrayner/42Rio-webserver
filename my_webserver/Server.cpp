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

Server::Server( void ) {}

Server	&Server::operator=( const Server &src )
{
	if (this != &src)
		this->_port = src._port;
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
}
