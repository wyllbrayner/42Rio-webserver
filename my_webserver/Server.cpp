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

}

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


int &	Server::getPort( void )
{
	return (this->_port);
}

void					Server::setPort( const std::string & _p )
{
	int 	tmp;
	size_t	i;
	
	tmp = 0;
	i = 0;
	while (i < _p.size())
	{
		if ( !std::isdigit(_p[i]) )
			throw Error::InvalidParameter();
		i++;
	}
	if ( i > 5 ) // 0 maior valor para uma porta válida é 65535, logo possui no máximo 5 dígitos.
		throw Error::InvalidParameter();
	tmp = std::atoi(_p.c_str());
	if (tmp > 65535 || tmp < 0)
		throw Error::InvalidParameter();
	else
		this->_port = tmp;
	tmp = 0;
}