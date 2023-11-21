/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:23:54 by woliveir          #+#    #+#             */
/*   Updated: 2022/05/09 15:09:49 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

WebServer::WebServer(void)
{
}

WebServer	&WebServer::operator=(const WebServer &src)
{
	if (this != &src)
	{
		return (*this);
	}
	else
		return (*this);
}

WebServer::WebServer(const WebServer& copy)
{
	*this = copy;
	return ;
}

WebServer::~WebServer(void)
{
}

