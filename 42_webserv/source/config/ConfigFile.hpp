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
# include "Location.hpp"

typedef std::vector<std::string>::iterator	myItVecS;
typedef std::vector<std::string> 			myVecS;

class	ConfigFile
{
	private:
		int							_port;
		in_addr_t					_host;
		myVecS						_server_name;
		myVecS						_index;
		std::string					_root;
		std::vector<Location>		_vec_location;

		bool		isTokenValid( std::string & _p );
		bool		isHostValid( std::string & _parameter );
		void		putVecString(myItVecS &i, std::vector<std::string> & sp_server, myVecS & _vecString );
	
	public:
		ConfigFile( void );
		ConfigFile( const ConfigFile& copy );
		ConfigFile	&operator=( const ConfigFile &src );
		~ConfigFile( void );

		const int 		&					getPort(void) const;
		const in_addr_t &					getHost(void) const;
		const myVecS &						getServerName(void) const;
		const myVecS &						getIndex(void) const;
		const std::string &					getRoot(void) const;
		const std::vector<Location>	&		getLocation(void) const;
		void								setPort(std::string & _p);
		void								setHost(std::string _parameter);
		void								setServerName(myItVecS &i, myVecS & sp_server);
		void								setIndex(myItVecS &i, myVecS & sp_server);
		void								setRoot(std::string & _parameter);
		void								setLocation(myItVecS &i, myVecS & sp_server);
};