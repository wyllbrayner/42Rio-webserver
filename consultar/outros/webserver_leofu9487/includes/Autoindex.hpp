#pragma once
#include "webserv.hpp"

class Autoindex
{
	private:
		std::string _html;

	public:
		Autoindex(std::string uri, std::string const &root);
		std::string const &get_html() const;
};