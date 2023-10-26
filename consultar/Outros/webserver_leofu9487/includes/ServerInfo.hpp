/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerInfo.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 17:01:26 by xli               #+#    #+#             */
/*   Updated: 2021/11/30 15:08:34 by yfu              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "webserv.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

class Location
{
	private:
		/*
		** Location attributes
		*/
		int									_port;
		bool								_autoindex;
		std::string							_uri;
		std::string							_index;
		std::string							_root;
		std::string							_redirect;
		std::vector<std::string>			_allow_method;
		std::string							_upload_path;
		std::map<std::string, std::string>	_cgi;

	public:
		Location(int);
		Location(const Location &copy);
		Location &operator=(const Location &copy);
		~Location();

		/*
		** Location getters
		*/
		int									const &get_port() const;
		std::string							const &get_uri() const;
		bool								const &get_autoindex() const;
		std::string							const &get_index() const;
		std::string							const &get_redirect() const;
		std::vector<std::string>			const &get_allow_method() const;
		std::string							const &get_root() const;
		std::string							const &get_upload_path() const;
		std::map<std::string, std::string>	const &get_cgi() const;

		void	set_location(int index, const int &pos, const std::string &str);

		void	print() const;

	private:
		/*
		** Location setters
		*/
		void	set_uri(const char *);
		void	set_autoindex(const char *);
		void	set_index(const char *);
		void	set_redirect(const char *);
		void	set_allow_method(const char *);
		void	set_root(const char *);
		void	set_upload_path(const char *);
		void	set_cgi(const char *);
};

class ServerInfo
{
	private:
		/*
		** Server attributes
		*/
		int							_port;
		std::string					_server_name;
		std::string					_IP;
		std::string					_error_pages;
		int							_client_body_size;
		std::vector<Location>		_location;

	public:
		ServerInfo();
		ServerInfo(const ServerInfo &copy);
		ServerInfo &operator=(const ServerInfo &copy);
		~ServerInfo();

		void						add_location(Location &);

		/*
		** Server getters
		*/
		int							const &get_port() const;
		std::string					const &get_name() const;
		std::string					const &get_IP() const;
		std::string					const &get_error() const;
		int							const &get_client_body_size() const;
		std::vector<Location>		const &get_location() const;

		void						set_server(int index, const int &pos, const std::string &str);
		std::vector<Location>		set_server_location();

		void	print() const;

	private:
		/*
		** Server setters
		*/
		void						set_port(const char *);
		void						set_name(const char *);
		void						set_error(const char *);
		void						set_client_body_size(const char *);
};


/*
Fill the attributes that can be indicated in conf file

getters for all the attributes

public method :
	print :
		print all the attributes via std::cerr, output must be readable

*/
