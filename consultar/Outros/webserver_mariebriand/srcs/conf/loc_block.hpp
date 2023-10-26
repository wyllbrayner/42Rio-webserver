/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loc_block.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 14:44:20 by vmoreau           #+#    #+#             */
/*   Updated: 2022/02/02 16:42:27 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOC_BLOCK_HPP
#define LOC_BLOCK_HPP

#include "../../incs/webserv.hpp"

#include <vector>
#include <map>

class loc_block
{
private:
	std::vector<std::string>	_block;
	std::string					_path_conf;

	std::string					_path;
	std::string					_root;
	std::vector<std::string>	_method_limit;
	std::string					_index;

public:
	loc_block(/* args */);
	~loc_block();
	loc_block& operator= (const loc_block);

	// PARSING
	void pars_loc(std::vector<std::string> block, std::string path);

	// SETTER
	void set_path(std::string value);
	void set_root(std::string value);
	void set_method_limit(std::string value);
	void set_index(std::string value);

	// GETTER
	std::string get_path() const 					{ return (this->_path); }
	std::string get_root() const 					{ return (this->_root); }
	std::string	get_index() const 					{ return (this->_index); }
	std::vector<std::string>	get_method_limit() const { return (this->_method_limit); }

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
	class NoPathFound : public std::exception
	{
		private:
			std::string _msg;
		public:
			NoPathFound(std::string path) : _msg(" Path is missing in " + path){}
			virtual ~NoPathFound() throw() {}
			virtual const char *what() const throw() {return (this->_msg.c_str());}
	};
	class NoRootFound : public std::exception
	{
		private:
			std::string _msg;
		public:
			NoRootFound(std::string path) : _msg(" Root is missing in " + path){}
			virtual ~NoRootFound() throw() {}
			virtual const char *what() const throw() {return (this->_msg.c_str());}
	};
	class NoIndexFound : public std::exception
	{
		private:
			std::string _msg;
		public:
			NoIndexFound(std::string path) : _msg(" Index is missing in " + path){}
			virtual ~NoIndexFound() throw() {}
			virtual const char *what() const throw() {return (this->_msg.c_str());}
	};
};

#endif
