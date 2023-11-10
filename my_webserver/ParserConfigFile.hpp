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
# include <string>
# include <iostream> //confirmar se será necessário para a entrega
# include "Error.hpp"
# include "Utils.hpp"
# include "ConfigFile.hpp"

class	ParserConfigFile
{
	private:
		ParserConfigFile(const ParserConfigFile& copy);
		ParserConfigFile	&operator=(const ParserConfigFile &src);
		std::vector<ConfigFile>	_servers;
		size_t					_nbrServers;

		void  	removeComents(std::string & line);
		void	splitServers(std::string & servers);
		void	findStartServer(const std::string & servers, size_t & start);
		void	findEndServer(const std::string & servers, size_t & end);
		void	buildServer(const std::string & server);
	
	public:
		ParserConfigFile(void);
		~ParserConfigFile(void);

		const	std::vector<ConfigFile>	&	getServers(void) const;
		const	size_t				&		getNbrServers(void) const;
		void								createServer(const std::string & config_path);
		void								printServer(void);
};