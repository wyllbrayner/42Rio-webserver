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
# include <iostream>
# include <vector>
# include "../Error.hpp"
# include "../Utils.hpp"
# include "ConfigFile.hpp"

class	ParserServer
{
	private:
		std::vector<ConfigFile>				_servers;
		size_t								_nbrServers;
		std::vector<ConfigFile>			&	getServersSmart(void);
		void								setServers(std::vector<ConfigFile> \
												& _parameter);
		void								removeComents(std::string & line);
		void								splitServers(std::string & servers);
		void								findStartServer(const std::string 	
												& servers, size_t & start);
		void								findEndServer(const std::string \
												& servers, size_t & end);
		void								buildServer(const std::string \
												& server);
		void								buildSingleServer(void);
		void								findReturn(void);
		void								removDuplic(void);
		bool								equalServerName(const size_t & i, \
												const size_t & j) const;

	public:
		ParserServer(void);
		ParserServer(const ParserServer & copy);
		ParserServer					&	operator=(const ParserServer & src);
		~ParserServer(void);
		const std::vector<ConfigFile>	&	getServers(void) const;
		const size_t					&	getNbrServers(void) const;
		void								createServer(void);
		void								createServer(const std::string \
												& config_path);
		void								print(void) const;
};