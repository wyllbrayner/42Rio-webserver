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
# include <vector>
# include <algorithm>
# include <set>
# include <strings.h> //bzero
# include "Error.hpp"
# include "Utils.hpp"
# include "Location.hpp"

typedef std::vector<std::string>::iterator	myItVecS;
typedef std::vector<std::string> 			myVecS;

class	Server
{
	private:
		std::vector<int>				_port;
		in_addr_t						_host;
		myVecS							_server_name;
		myVecS							_index;
		std::string						_root;
		std::vector<Location>			_vec_location;
		std::vector<int>				_fdListen;
		std::vector<struct sockaddr_in>	_serverAddress;

		bool	isTokenValid( std::string & _p );
		bool	isHostValid( std::string & _parameter );
		void	putVecString(myItVecS &i, std::vector<std::string> & sp_server, myVecS & _vecString );
		void	putVecInt(std::string & _parameter);
		bool	portIsDuplic(void) const;

	public:
		Server(void);
		Server	&operator=(const Server &src);
		Server(const Server& copy);
		~Server(void);

		const std::vector<int> &					getPort(void) const;
		const int &									getPort(const size_t & _port) const;
		const in_addr_t &							getHost(void) const;
		const myVecS &								getServerName(void) const;
		const std::string &							getServerName(const size_t & _i) const;
		const myVecS &								getIndex(void) const;
		const std::string &							getIndex(const size_t & _i) const;
		const std::string &							getRoot(void) const;
		const std::vector<Location> &				getLocation(void) const;
		const Location &							getLocation(const size_t & _i) const;
		const std::vector<int> &					getFDListen(void) const;
		const std::vector<struct sockaddr_in> &		getServerAddress(void) const;
		void										setPort(myItVecS &i, myVecS & sp_server);
		void										setHost(std::string _parameter);
		void										setServerName(myItVecS &i, myVecS & sp_server);
		void										setServerNameSmart(std::string _parameter);
		void										setIndex(myItVecS &i, myVecS & sp_server);
		void										setIndexSmart(std::string _parameter);
		void										setRoot(std::string _parameter);
		void										setLocation(myItVecS &i, myVecS & sp_server);
		void										setupServer(void);
		void										destroyServer(void);
};