/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_url.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 15:59:47 by vmoreau           #+#    #+#             */
/*   Updated: 2022/02/18 04:08:47 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
/*
	---------------------- UTILS ----------------------
*/
std::vector<loc_block>::iterator	Request::location_found(std::string url, std::vector<loc_block> *loc)
{
	std::string tmp;

	for (std::vector<loc_block>::iterator it = loc->begin(); it != loc->end(); it++)
	{
		if (url.size() > it->get_path().size())
			tmp.assign(url.begin(), url.begin() + it->get_path().size());
		else
			tmp.assign(url.begin(), url.end());

		// std::cout << tmp << "   " << it->get_path() << '\n';

		if (it->get_path() == tmp)
			return (it);
	}
	return (loc->end());
}

bool		Request::is_url_dir(std::string url)
{
	std::string::size_type pos = url.find_last_of('.');

	return (pos == url.npos);
}

bool		Request::is_method_available(std::string url)
{
	std::vector<loc_block> loc = this->_block->get_location();
	std::string ret;

	std::vector<loc_block>::iterator it = location_found(url, &loc);

	if (it != loc.end())
	{
		std::vector<std::string> vec_method = it->get_method_limit();
		for (std::vector<std::string>::iterator it2 = vec_method.begin(); it2 != vec_method.end(); it2++)
		{
			if (*it2 == this->_method)
				return (true);
		}
	}
	else if (this->_method == "GET" || this->_method == "POST" || this->_method == "DELETE")
		return (true);

	return (false);
}

/*
	---------------------- PROCESSING ----------------------
*/
void		Request::treatUrl()
{
	this->_url.insert(0, 1, '.');
	this->_url_dir = is_url_dir(this->_base_url);

	if (this->_referer.empty() == true)
	{
		if (this->_response_status_code == 0)
		{
			if (this->is_method_available(this->_base_url) == false)
				this->_response_status_code = 405;
			else if (this->_method != "GET" && this->_method != "POST" && this->_method != "DELETE")
				this->_response_status_code = 418;
		}
		if (this->_base_url == "/")
			this->_url += this->treat_void_url();
		else if (this->_url_dir == true)
			this->_url += this->treat_dir_url(this->_base_url);
		else
			this->_url += this->treat_full_url(this->_base_url);
	}
	else
		this->treatUrl_with_referer();
}

/*
	Treat Url if requested URL id "/"
*/
std::string	Request::treat_void_url()
{
	std::vector<loc_block> loc = this->_block->get_location();
	std::string ret;

	for (std::vector<loc_block>::iterator it = loc.begin(); it != loc.end(); it++)
	{
		if (it->get_path() == "/")
		{
			ret += "/" + it->get_root() + it->get_index();
			return (ret);
		}
	}
	ret += "/" + this->_block->get_default_root() + this->_block->get_default_path();

	return (ret);
}

/*
	Treat Url if requested URL is a Directory
*/
std::string	Request::treat_dir_url(const std::string url)
{
	std::vector<loc_block> loc = this->_block->get_location();
	std::string ret;

	std::vector<loc_block>::iterator it = location_found(url, &loc);

	if (it != loc.end())
	{
		// std::cout << "PASS DIR IF\n";

		std::string tmp(url.begin() + it->get_path().size(), url.end());
		ret += "/" + it->get_root() + tmp;
		if (ret.back() != '/')
			ret += "/";
		ret += it->get_index();
	}
	else
	{
		// std::cout << "PASS DIR ELSE\n";

		ret +=  "/" + this->_block->get_default_root() + url;
		if (ret.back() != '/')
			ret += "/";
		ret += this->_block->get_default_path();
	}

	return (ret);
}

/*
	Treat Url if requested URL is a full path
*/
std::string	Request::treat_full_url(const std::string url)
{
	std::vector<loc_block> loc = this->_block->get_location();
	std::string ret;

	std::vector<loc_block>::iterator it = location_found(url, &loc);

	if (it != loc.end())
	{
		// std::cout << "PASS FULL IF\n";

		std::string tmp(url.begin() + it->get_path().size(), url.end());
		ret += "/" + it->get_root() + tmp;
	}
	else
	{
		// std::cout << "PASS FULL ELSE\n";

		ret +=  "/" + this->_block->get_default_root() + url;
	}

	return (ret);
}
