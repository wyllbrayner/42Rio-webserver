/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serv_block.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 14:44:09 by vmoreau           #+#    #+#             */
/*   Updated: 2022/02/16 18:29:29 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERV_BLOCK_HPP
#define SERV_BLOCK_HPP

#include "../../incs/webserv.hpp"

#include <vector>
#include <map>

#include "loc_block.hpp"

#define BODY_DEFAULT_SIZE 5000

class serv_block
{
private:
	std::string								_path;
	std::vector<std::string>				_block;
	std::map<std::string, std::string>		_error_page;
	std::vector< std::vector<std::string> >	_loc_block;
	int										_port;
	std::string								_host;
	std::string								_server_name;
	int										_client_max_body_size;
	std::vector<loc_block>					_location;
	std::string								_default_root;
	std::string								_default_path;
	bool									_autoindex;

public:
	serv_block();
	~serv_block();

	// PARSING
	void pars_serv(std::vector<std::string> block, std::string path, std::map<std::string, std::string> error_page);

	// SETTER
	void set_port_host(std::string value);
	void set_server_name(std::string value);
	void set_client_max_body_size(std::string value);
	void set_default_root(std::string value);
	void set_default_path(std::string value);
	void set_autoindex(std::string value);

	// GETTER
	std::string get_host() const					{ return (this->_host); }
	std::string get_server_name() const				{ return (this->_server_name); }
	int get_port() const							{ return (this->_port); }
	int get_client_max_body_size() const			{ return (this->_client_max_body_size); }
	std::vector<loc_block> get_location() const		{ return (this->_location); }
	std::string get_default_root() const			{ return (this->_default_root); }
	std::string get_default_path() const			{ return (this->_default_path); }
	std::map<std::string, std::string> get_error_page() const	{ return (this->_error_page); }
	bool get_autoindex() const 						{ return (this->_autoindex); }



	// EXCEPTION
	class ConfFile : public std::exception
	{
		private:
			std::string _msg;
		public:
			ConfFile(std::string msg) : _msg(msg){}
			virtual ~ConfFile() throw() {}
			virtual const char *what() const throw() {return (this->_msg.c_str());}
	};
	class NoListenFound : public std::exception
	{
		private:
			std::string _msg;
		public:
			NoListenFound(std::string path) : _msg(" Listen is missing in " + path){}
			virtual ~NoListenFound() throw() {}
			virtual const char *what() const throw() {return (this->_msg.c_str());}
	};
	class NoS_NameFound : public std::exception
	{
		private:
			std::string _msg;
		public:
			NoS_NameFound(std::string path) : _msg(" Server_name is missing in " + path){}
			virtual ~NoS_NameFound() throw() {}
			virtual const char *what() const throw() {return (this->_msg.c_str());}
	};
	class NoPortFound : public std::exception
	{
		private:
			std::string _msg;
		public:
			NoPortFound(std::string path) : _msg(" Port is missing in " + path){}
			virtual ~NoPortFound() throw() {}
			virtual const char *what() const throw() {return (this->_msg.c_str());}
	};
	class NoLocationFound : public std::exception
	{
		private:
			std::string _msg;
		public:
			NoLocationFound(std::string path) : _msg(" No Location block found in " + path){}
			virtual ~NoLocationFound() throw() {}
			virtual const char *what() const throw() {return (this->_msg.c_str());}
	};
// Private Function
private:
	std::vector<std::string> save_loc_block(std::vector<std::string> tmp, size_t *pos);
	std::vector<std::string> separate_loc_block(std::vector<std::string> tmp, size_t nb_loc);
	void check_host(std::string value);
	void sort_location_block();
};

#endif
