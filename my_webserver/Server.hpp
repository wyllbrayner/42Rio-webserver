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
# include <fcntl.h>
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
		myVecS							_serverName;
		myVecS							_index;
		std::string						_root;
		std::vector<Location>			_vecLocation;
		std::vector<int>				_socket;
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
		const int	&								getPortUnic(void) const;
		const in_addr_t &							getHost(void) const;
		const myVecS &								getServerName(void) const;
		const std::string &							getServerName(const size_t & _i) const;
		const std::string &							getServerNameUnic(void) const;
		const myVecS &								getIndex(void) const;
		const std::string &							getIndex(const size_t & _i) const;
		const std::string &							getIndexUnic(void) const;
		const std::string &							getRoot(void) const;
		const std::vector<Location> &				getLocation(void) const;
		const Location &							getLocation(const size_t & _i) const;
		const std::vector<int> &					getSocket(void) const;
		const std::vector<struct sockaddr_in> &		getServerAddress(void) const;
		void										setPort(myItVecS &i, myVecS & sp_server);
		void										setPort(const int & _port);
		void										setHost(std::string _parameter);
		void										setHost(const in_addr_t & host);
		void										setServerName(const myVecS _sn);
		void										setServerName(myItVecS &i, myVecS & sp_server);
		void										setServerNameSmart(std::string _parameter);
		void										setIndex(const myVecS & _idx);
		void										setIndex(myItVecS &i, myVecS & sp_server);
		void										setIndexSmart(std::string _parameter);
		void										setRootUnic(const std::string & _rt);
		void										setRoot(std::string _parameter);
		void										setLocation(myItVecS &i, myVecS & sp_server);
		void										setLocation(const std::vector<Location> & _loc);
		void										setupServer(void);
		void										destroyServer(void);
};