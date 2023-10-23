/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:23:54 by woliveir          #+#    #+#             */
/*   Updated: 2022/05/09 15:09:49 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma	once

# include <netinet/in.h>
# include "Error.hpp"

class	Server
{
	private:
		int	_port;
/*
		in_addr_t			_host;
		std::string			_server_name;
		std::string			_root;
		unsigned long int	_client_max_body_size;
		std::string			_index;
		bool				_autoindex;
		int					_fd_sockaddr;
		struct sockaddr_in	_server_addr;
*/
	
	public:
		Server( void );
		Server( const Server& copy );
		Server	&operator=( const Server &src );
		~Server( void );

		int & getPort( void );
		void				setPort( const std::string & _p );
};