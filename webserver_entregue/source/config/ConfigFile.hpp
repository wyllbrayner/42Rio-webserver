/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:23:54 by woliveir          #+#    #+#             */
/*   Updated: 2022/05/09 15:09:49 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include <string>
# include <iostream>
# include <arpa/inet.h>
# include <set>
# include <vector>
# include <algorithm>    // std::find
# include "../Error.hpp"
# include "../Utils.hpp"
# include "Location.hpp"

typedef std::vector<std::string>::iterator	myItVecS;
typedef std::vector<std::string>			myVecS;

class	ConfigFile
{
	private:
		bool							_serverDefault;
		std::vector<int>				_port;
		std::string						_root;
		myVecS							_serverName;
		in_addr_t						_host;
		myVecS							_index;
		int								_maxBodySize;
		myVecS							_errorPage;
		std::vector<Location>			_vecLocation;
		bool							isTokenValid(std::string & _p);
		bool							isHostValid(std::string & _parameter);
		void							putVecString(myItVecS & i, \
										std::vector<std::string> & sp_server, \
										myVecS & _vecString);
		void							putVecInt(std::string & _parameter);
		bool							portIsDuplic(void) const;
		ssize_t							findReturn(\
				const std::string & _return, const size_t & i, size_t limit);
	
	public:
		ConfigFile(void);
		ConfigFile(const ConfigFile & copy);
		ConfigFile					&	operator=(const ConfigFile & src);
		~ConfigFile(void);
		const bool					&	getIsServerDefault(void) const;
		const std::vector<int>		&	getPort(void) const;
		const in_addr_t				&	getHost(void) const;
		const myVecS				&	getServerName(void) const;
		const myVecS				&	getIndex(void) const;
		const std::string			&	getRoot(void) const;
		const int					&	getMaxBodySize(void) const;
		const myVecS				&	getErrorPage(void) const;
		const std::vector<Location>	&	getLocation(void) const;
		void							setServerDefaultSmart(bool _parameter);
		void							setPort(myItVecS & i, \
										myVecS & sp_server);
		void							setPortSmart(std::string _parameter);
		void							setPort(int _parameter);
		void							setHost(std::string _parameter);
		void							setHost(const in_addr_t & _parameter);
		void							setServerName(myItVecS & i, \
										myVecS & sp_server);
		void							setServerName(const myVecS & _parameter);
		void							setServerNameSmart(std::string _parameter);
		void							setIndex(myItVecS & i, myVecS & sp_server);
		void							setIndexSmart(std::string _parameter);
		void							setIndex(const myVecS & _parameter);
		void							setRoot(std::string _parameter);
		void							setRootSmart(const std::string & _parameter);
		void							setMaxBodySize(std::string _parameter);
		void							setMaxBodySize(const int & _parameter);
		void							setErrorPage(myItVecS & i, myVecS & sp_server);
		void							setErrorPageSmart(std::string _parameter);
		void							setErrorPage(const myVecS & _parameter);
		void							setLocation(myItVecS & i, \
										myVecS & sp_server, const std::string & root, \
										const std::string & index);
		void							setLocation(\
										const std::vector<Location> & _parameter);
		void							fixeErrorPage(void);
		void							printConfigFile(void) const;
		void							clearConfFile(void);
		void							findLastReturn(void);
		
		template<typename T>
		void							printVec(const T & content, \
										std::string str) const;
};

template<typename T>
void									ConfigFile::printVec(\
									const T & content, std::string str) const
{
	size_t	i;

	i = 0;
	std::cout << str;
	while (i < content.size())
	{
		std::cout << content[i];
		if ((i + 1) < content.size())
			std::cout << ", ";
		i++;
	}
	std::cout << std::endl;
}