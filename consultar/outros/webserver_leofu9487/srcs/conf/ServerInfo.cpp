/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerInfo.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xli <xli@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 17:01:04 by xli               #+#    #+#             */
/*   Updated: 2021/12/01 09:02:39 by xli              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ServerInfo.hpp"

/*
** =============== LOCATION ===============
*/

Location::Location(int p)
:	_port(p),
	_autoindex(0),
	_index(""),
	_root(""),
	_redirect("")
{
	const char *method_list[] = { "POST", "GET", "DELETE"};
	_allow_method.insert(_allow_method.begin(), method_list, method_list + 3);
}

Location::Location(const Location &copy)
:	_port(copy._port),
	_autoindex(copy._autoindex),
	_uri(copy._uri),
	_index(copy._index),
	_root(copy._root),
	_redirect(copy._redirect),
	_allow_method(copy._allow_method),
	_upload_path(copy._upload_path),
	_cgi(copy._cgi) {}

Location &Location::operator=(const Location &copy)
{
	Location	tmp(copy);
	std::swap(tmp, *this);
	return (*this);
}
Location::~Location() {}

/*
** --------------- Getter ---------------
*/

int const &Location::get_port() const { return _port; }

std::string	const &Location::get_uri() const { return _uri;}

bool const &Location::get_autoindex() const { return _autoindex; }

std::string const &Location::get_index() const { return _index; }

std::string const &Location::get_redirect() const { return _redirect; }

std::vector<std::string> const &Location::get_allow_method() const { return _allow_method; }

std::string const &Location::get_root() const { return _root; }

std::string const &Location::get_upload_path() const { return _upload_path; }

std::map<std::string, std::string> const &Location::get_cgi() const { return _cgi; }

/*
** --------------- SETTER ---------------
*/



void Location::set_location(int index, const int &pos, const std::string &str)
{
	typedef void (Location::*location_funcs)(const char *);
	location_funcs	funcs[] = {&Location::set_uri, &Location::set_autoindex,
		&Location::set_index, &Location::set_root, &Location::set_redirect,
		&Location::set_allow_method, &Location::set_upload_path,
		&Location::set_cgi};

	const char *tmp = str.c_str() + pos;
	int i = 0;
	while (isspace(tmp[i]))
		i++;
	(this->*funcs[index])(tmp + i);
}

void Location::set_uri(const char *u)
{
	std::string	tmp = u;
	tmp.erase(std::remove_if(tmp.begin(), tmp.end(), isspace), tmp.end());
	_uri = tmp;
}

void Location::set_autoindex(const char *a)
{
	std::string tmp = a;
	tmp.erase(std::remove_if(tmp.begin(), tmp.end(), isspace), tmp.end());
	if (tmp.compare("on") && tmp.compare("off"))
		throw(ConfFileParseError("put only on or off for autoindex"));
	else if (!tmp.compare("on"))
		_autoindex = 1;
	else
		_autoindex = 0;
}

void Location::set_index(const char *i)
{
	if (nb_tokens(i) != 1)
		throw(ConfFileParseError("put only one index"));
	std::string tmp = i;
	tmp.erase(std::remove_if(tmp.begin(), tmp.end(), isspace), tmp.end());
	_index = tmp;
}

void Location::set_root(const char *r)
{
	if (nb_tokens(r) != 1)
		throw(ConfFileParseError("put only one root"));
	std::string	tmp = r;
	tmp.erase(std::remove_if(tmp.begin(), tmp.end(), isspace), tmp.end());
	_root = tmp;
}

void Location::set_redirect(const char *r)
{
	if (nb_tokens(r) != 1)
		throw(ConfFileParseError("put only one redirect path"));
	std::string	tmp = r;
	tmp.erase(std::remove_if(tmp.begin(), tmp.end(), isspace), tmp.end());
	_redirect = tmp;
}

void Location::set_allow_method(const char *m)
{
	_allow_method.clear();
	std::stringstream	str(m);
	std::string	token;
	while (str >> token)
		_allow_method.push_back(token);
	const char *method_check[] = { "POST", "GET", "DELETE"};
	for (size_t i = 0; i < _allow_method.size(); i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			if (!strcmp(method_check[j], _allow_method[i].c_str()))
				break;
			if (j == 2)
				throw(ConfFileParseError("put only allowed methods"));
		}
	}
}

void Location::set_upload_path(const char *u)
{
	if (nb_tokens(u) != 1)
		throw(ConfFileParseError("put only one upload path"));
	std::string	tmp = u;
	tmp.erase(std::remove_if(tmp.begin(), tmp.end(), isspace), tmp.end());
	_upload_path = tmp;
}

void Location::set_cgi(const char *c)
{
	std::stringstream	str(c);
	std::vector<std::string> tmp;
	std::string	token;
	while (str >> token)
		tmp.push_back(token);
	if (tmp.size() != 2)
		throw(ConfFileParseError("wrong input for cgi"));
	_cgi.insert(std::pair<std::string, std::string>(tmp[0], tmp[1]));
}

void Location::print() const
{
	std::cerr << "----------Location attributes----------" << std::endl;
	std::cerr << "_port = " << _port << std::endl;
	std::cerr << "_autoindex = " << _autoindex << std::endl;
	std::cerr << "_uri = " << _uri << std::endl;
	std::cerr << "_index = " << _index << std::endl;
	std::cerr << "_root = " << _root << std::endl;
	std::cerr << "_redirect = " << _redirect << std::endl;
	for (std::vector<std::string>::const_iterator it = _allow_method.begin(); it != _allow_method.end(); ++it)
		std::cerr << "_allow_method = " << *it << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = _cgi.begin(); it != _cgi.end(); ++it)
		std::cerr << "_cgi = " << it->first << " => " << it->second << std::endl;
}

/*
** =============== SERVERINFO ===============
*/

ServerInfo::ServerInfo()
:	_port(8080),
	_IP("0.0.0.0"),
	_client_body_size(0) {}

ServerInfo::ServerInfo(const ServerInfo &copy)
:	_port(copy._port),
	_server_name(copy._server_name),
	_IP(copy._IP),
	_error_pages(copy._error_pages),
	_client_body_size(copy._client_body_size),
	_location(copy._location) {}

ServerInfo &ServerInfo::operator=(const ServerInfo &copy)
{
	ServerInfo	tmp(copy);
	std::swap(tmp, *this);
	return (*this);
}

ServerInfo::~ServerInfo() {}

/*
** --------------- Getter ---------------
*/

int const &ServerInfo::get_port() const { return _port; }

std::string const &ServerInfo::get_name() const { return _server_name; }

std::string const &ServerInfo::get_IP() const { return _IP; }

std::string const &ServerInfo::get_error() const { return _error_pages; }

int const &ServerInfo::get_client_body_size() const { return _client_body_size; }

std::vector<Location> const &ServerInfo::get_location() const { return _location; }

/*
** --------------- SETTER ---------------
*/

void ServerInfo::set_server(int index, const int &pos, const std::string &str)
{
	typedef void (ServerInfo::*server_func)(const char *);
	server_func	funcs[] = {&ServerInfo::set_port, &ServerInfo::set_name,
		&ServerInfo::set_error, &ServerInfo::set_client_body_size};

	const char *tmp = str.c_str() + pos;
	int i = 0;
	while (isspace(tmp[i]))
		i++;
	(this->*funcs[index])(tmp + i);
}

void ServerInfo::set_port(const char *p)
{
	if (nb_tokens(p) != 1)
		throw(ConfFileParseError("put only one port to listen"));
	std::string	tmp = p;
	tmp.erase(std::remove_if(tmp.begin(), tmp.end(), isspace), tmp.end());
	for (size_t i = 0; i < tmp.size(); i++)
		if (tmp[i] < 48 || tmp[i] > 57)
			throw(ConfFileParseError("put only digits for port"));
	_port = atoi(tmp.c_str());
}

void ServerInfo::set_name(const char *n)
{
	if (nb_tokens(n) != 1)
		throw(ConfFileParseError("put only one server name"));
	std::string tmp = n;
	tmp.erase(std::remove_if(tmp.begin(), tmp.end(), isspace), tmp.end());
	_server_name = tmp;
}

void ServerInfo::set_error(const char *e)
{
	if (nb_tokens(e) != 1)
		throw(ConfFileParseError("put only one error path"));
	_error_pages = e;
}

void ServerInfo::set_client_body_size(const char *s)
{
	if (nb_tokens(s) != 1)
		throw(ConfFileParseError("put only one max client body size"));
	std::string	tmp = s;
	tmp.erase(std::remove_if(tmp.begin(), tmp.end(), isspace), tmp.end());
	for (size_t i = 0; i < tmp.size(); i++)
		if (tmp[i] < 48 || tmp[i] > 57)
			throw(ConfFileParseError("put only digits for max client body size"));
	_client_body_size = atoi(tmp.c_str());
}

std::vector<Location> ServerInfo::set_server_location() { return _location; }

void ServerInfo::add_location(Location &location) { _location.push_back(location); }

void ServerInfo::print() const
{
	std::cerr << "----------ServerInfo attributes----------" << std::endl;
	std::cerr << "_port = " << _port << std::endl;
	std::cerr << "_server_name = " << _server_name << std::endl;
	std::cerr << "_error_pages = " << _error_pages << std::endl;
	std::cerr << "_client_body_size = " << _client_body_size << std::endl;
	std::cerr << "_location : " << std::endl;
	for (size_t i = 0 ; i < _location.size() ; ++i)
	{
		std::cerr << std::endl;
		_location[i].print();
		std::cerr << std::endl;
	}
	std::cerr << std::endl;
}
