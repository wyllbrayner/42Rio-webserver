/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConfigFile.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:23:54 by woliveir          #+#    #+#             */
/*   Updated: 2022/05/09 15:09:49 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma	once

# include <fstream>
# include <iostream>
# include "Error.hpp"
# include "Utils.hpp"
# include "Server.hpp"

class	ParserConfigFile
{
	private:
		std::vector<Server>		_servers;
		size_t					_nbrServers;
		ParserConfigFile(const ParserConfigFile& copy);
		ParserConfigFile	&	operator=(const ParserConfigFile &src);

		void  	removeComents(std::string & line);
		void	splitServers(std::string & servers);
		void	findStartServer(const std::string & servers, size_t & start);
		void	findEndServer(const std::string & servers, size_t & end);
		void	buildServer(const std::string & server);
	
	public:
		ParserConfigFile(void);
		~ParserConfigFile(void);

		const	std::vector<Server>	&	getServers(void) const;
		const	size_t				&	getNbrServers(void) const;
		void							parserConfigFile(const std::string & config_path);
		void							setupServers(void);
		void							printServer(void) const;
};