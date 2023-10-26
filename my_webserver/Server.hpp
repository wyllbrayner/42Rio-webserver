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

# include <arpa/inet.h>

# include <unistd.h> //getcwd na setroot
# include <sys/types.h> //open/close dir na setroot
# include <dirent.h> //open/close dir na setroot

# include <vector>
# include "Error.hpp"
# include "Utils.hpp"

class	Server
{
	private:
		int							_port;
		in_addr_t					_host;
		std::vector<std::string>	_server_name;
		std::vector<std::string>	_page_server_name;
		std::string					_index;
		std::string					_root;
/*
		unsigned long int	_client_max_body_size;
		bool				_autoindex;
		int					_fd_sockaddr;
		struct sockaddr_in	_server_addr;
*/
		bool		isTokenValid( std::string & _p );
		bool		isHostValid( std::string & _parameter );
	
	public:
		Server( void );
		Server( const Server& copy );
		Server	&operator=( const Server &src );
		~Server( void );

		const int 		&					getPort(void) const;
		const in_addr_t &					getHost(void) const;
		const std::vector<std::string> &	getServerName(void) const;
		const std::string &					getIndex(void) const;
		const std::string &					getRoot(void) const;
		void								setPort(std::string & _p);
		void								setHost(std::string & _parameter);
		void								setServerName(std::vector<std::string>::iterator &i, std::vector<std::string> & sp_server);
		void								setIndex(std::string & _parameter);
		void								setRoot(std::string & _parameter);
};