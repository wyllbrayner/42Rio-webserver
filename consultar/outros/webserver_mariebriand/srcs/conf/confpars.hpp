/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   confpars.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 15:55:46 by vmoreau           #+#    #+#             */
/*   Updated: 2022/02/16 22:10:10 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFPARS_HPP
#define CONFPARS_HPP
#include "../../incs/webserv.hpp"

#include <vector>
#include <map>
#include <string>

#include "serv_block.hpp"

class confpars
{
private:
	std::ifstream				_fc;
	std::string					_path;
	std::vector<std::string>	_file;
	std::vector< std::vector<std::string> >	_server_block;

	std::vector<serv_block>		_server;
	std::map<std::string, std::string>		_error_page;
	std::string					_cgi_path;

public:
	confpars();
	~confpars();
	// confpars& operator=(const confpars conf);

	// FD MANIPULATION
	void open_fc(const char *path);
	void reopen_fc();
	void close_fc();

	// GETTER
	std::ifstream &get_fd()						{ return (this->_fc); }
	std::vector<serv_block> get_server() const	{ return (this->_server); }
	std::map<std::string, std::string> get_error_page() const { return (this->_error_page); }
	std::string get_cgi_path() const			{ return (this->_cgi_path); }

	// SETTER
	void set_error_page(std::string value);
	void set_cgi_path(std::string value);

	// PARSING
	void pars_fc(std::ifstream &fc);
	void pars_http();

	// EXEPTIONS
	class FileNotFound : public std::exception
	{
		private:
			std::string _msg;
		public:
			FileNotFound(std::string path) : _msg(path + " Not Found"){}
			virtual ~FileNotFound() throw() {}
			virtual const char *what() const throw() {return (this->_msg.c_str());}
	};
	class ConfFile : public std::exception
	{
		private:
			std::string _msg;
		public:
			ConfFile(std::string msg) : _msg(msg){}
			virtual ~ConfFile() throw() {}
			virtual const char *what() const throw() {return (this->_msg.c_str());}
	};
	class NoServerFound : public std::exception
	{
		private:
			std::string _msg;
		public:
			NoServerFound(std::string path) : _msg(" No Server block found in " + path){}
			virtual ~NoServerFound() throw() {}
			virtual const char *what() const throw() {return (this->_msg.c_str());}
	};
	class NoErr_PageFound : public std::exception
	{
		private:
			std::string _msg;
		public:
			NoErr_PageFound(std::string path) : _msg(" Error_page is missing in " + path){}
			virtual ~NoErr_PageFound() throw() {}
			virtual const char *what() const throw() {return (this->_msg.c_str());}
	};

	// Private Function
	private:
		std::vector<std::string>	separate_server_block(std::vector<std::string> tmp, size_t nb_serv);
		std::vector<std::string>	save_server_block(std::vector<std::string> tmp, size_t *pos);
		bool						server_port_unique();
		void						check_minimal_error_page();
		bool						is_path_error_page_ok();
};

#endif
