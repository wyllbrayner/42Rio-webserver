/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:23:54 by woliveir          #+#    #+#             */
/*   Updated: 2022/05/09 15:09:49 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma	once

# include "ParserConfigFile.hpp"
# include <map>

class	WebServer
{
	private:
		ParserConfigFile		_configFile;
		std::vector<Server>		_cluster;
		size_t					_nbrServers;
        int						_biggestSocket;
        fd_set					_recvSocketSet;
        fd_set					_writeSocketSet;
		std::map<int, Server>	_mapServers;

		void						buildCluster(const std::vector<Server> & _servers);
		void						setupCluster(void);
		void						fillServer(const Server & _server, const size_t & _j);
		void						initSets(void);
		void						runCluster(void);
		void						addToSet(const int & port, fd_set & recvSocket);

	public:
		WebServer(void);
		WebServer	&operator=(const WebServer &src);
		WebServer(const WebServer& copy);
		~WebServer(void);

		void					manager(const std::string & config_path);
		void					printCluster(void) const;
};