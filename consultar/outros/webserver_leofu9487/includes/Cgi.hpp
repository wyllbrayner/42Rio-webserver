#pragma once
#include "webserv.hpp"

// class Location;

class Cgi
{
private:
	std::string _msg;
	status_code _status_code;
public:
	static bool	is_cgi(std::map<std::string, std::string> const & cgi_map, std::string const &file_name);
	Cgi(Location const &location, HTTPRequest const &request);

	std::string const &get_msg() const;
	status_code get_status_code() const;
};
