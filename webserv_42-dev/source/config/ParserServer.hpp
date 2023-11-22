/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:23:54 by woliveir          #+#    #+#             */
/*   Updated: 2022/05/09 15:09:49 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma	once

# include <fstream>
# include <string>
# include <iostream> //confirmar se será necessário para a entrega
# include "Error.hpp"
# include "Utils.hpp"
# include "ConfigFile.hpp"

class	ParserServer
{
	private:
		std::vector< ConfigFile >	_servers;
		size_t						_nbrServers;

		void  	removeComents( std::string & line );
		void	splitServers( std::string & servers );
		void	findStartServer( const std::string & servers, size_t & start );
		void	findEndServer( const std::string & servers, size_t & end );
		void	buildServer( const std::string & server );
	
	public:
		ParserServer( void );
		ParserServer( const ParserServer& copy );
		~ParserServer( void );
		ParserServer	&operator=( const ParserServer &src );

		const std::vector< ConfigFile > &	getServers(void) const;
		const size_t 					&	getNbrServers(void) const;
		void	createServer( const std::string & config_path);
};