/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_referer.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 16:50:40 by vmoreau           #+#    #+#             */
/*   Updated: 2022/02/18 23:59:51 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

/*
	---------------------- UTILS ----------------------
*/

std::string trim_loc_ref(std::string ref, std::string loc_path)
{
	std::string ret(ref.begin() + loc_path.size(), ref.end());

	if (ret.back() != '/')
		ret.push_back('/');
	return ret;
}

bool IsRefPathExist(const std::string &s)
{
	struct stat buffer;
	return (stat (s.c_str(), &buffer) == 0);
}

bool		Request::is_referer_autoindex(std::string url)
{
	std::string url_dir(url.begin(), url.begin() + url.find_last_of('/'));
	bool path_exist = IsRefPathExist(url_dir);

	if (url_dir == ".")
		path_exist = false;

	if (this->_block->get_autoindex() && path_exist)
		return (true);
	return (false);
}

bool		Request::is_referer_error(std::string *ref_code, bool *ref_autoindex)
{
	std::string tmp_url(this->_base_url.begin(), this->_base_url.begin() + this->_base_url.find_last_of('/'));

	if (tmp_url.front() == '/')
		tmp_url.erase(tmp_url.begin());

	if (tmp_url.empty() == false)
	{
		if (tmp_url == "$ERROR$")
		{
			*ref_code = "400";
			return (true);
		}
	}

	std::string test_url = ".";
	bool url_dir = is_url_dir(this->_referer);

	if (this->_referer == "/")
		test_url += this->treat_void_url();
	else if (url_dir == true)
		test_url += this->treat_dir_url(this->_referer);
	else
		test_url += this->treat_full_url(this->_referer);

	// std::cout << test_url << '\n';
	std::ifstream ms;

	ms.close();
	ms.open(test_url);
	if (ms.is_open() == true)
		*ref_code = "200";
	else
		*ref_code = "404";
	ms.close();

	if (*ref_code == "404" && url_dir == true)
	{
		if (is_referer_autoindex(test_url) == true)
		{
			*ref_code = "200";
			*ref_autoindex = true;
		}
	}
	return (*ref_code == "404" ? true : false);
}

/*
	---------------------- PROCESSING ----------------------
*/

void		Request::treatUrl_with_referer()
{
	// std::cout << RED << "Referer-> " << this->_referer << NC << "\n\n";

	std::string ref_code;
	bool ref_autoindex = false;
	bool err_ref = is_referer_error(&ref_code, &ref_autoindex);

	if (ref_autoindex == true || this->_method != "GET")
	{
		if (this->is_method_available(this->_base_url) == false)
		{
			err_ref = true;
			ref_code = "405";
			this->_response_status_code = 405;
		}
		else if (this->_method != "GET" && this->_method != "POST" && this->_method != "DELETE")
		{
			err_ref = true;
			ref_code = "418";
			this->_response_status_code = 418;
		}
	}

	if (err_ref)
		this->treatUrl_with_err_referer(ref_code);
	else if (this->_referer == "/")
		this->treatUrl_with_void_referer();
	else if (is_url_dir(this->_referer) == true)
		this->treatUrl_with_dir_referer();
	else
		this->treatUrl_with_full_referer();
}

void		Request::treatUrl_with_void_referer()
{
	std::vector<loc_block> loc = this->_block->get_location();
	std::vector<loc_block>::iterator it = location_found(this->_base_url, &loc);
	std::string ret;

	if (it != loc.end() && it->get_path() != "/")
	{
		std::string test_url = this->treat_full_url(this->_base_url);
		this->_url += test_url;
		if (this->is_url_dir(this->_base_url) == true)
			this->_url += "/" +  it->get_index();
		return ;
	}
	for (std::vector<loc_block>::iterator it = loc.begin(); it != loc.end(); it++)
	{
		if (it->get_path() == "/")
		{
			std::string tmp(this->_base_url.begin() + it->get_path().size(), this->_base_url.end());
			ret += "/" + it->get_root() + tmp;

			if (this->is_url_dir(this->_base_url) == true)
				ret += "/" +  it->get_index();
			this->_url += ret;
			return ;
		}
	}
	ret += "/" + this->_block->get_default_root() + this->_base_url;
	if (this->is_url_dir(this->_base_url) == true)
		ret += "/" + this->_block->get_default_path();

	this->_url += ret;
	return;
}

void		Request::treatUrl_with_dir_referer()
{
	std::vector<loc_block> loc = this->_block->get_location();
	std::vector<loc_block>::iterator it = location_found(this->_referer, &loc);
	std::vector<loc_block>::iterator it2 = location_found(this->_base_url, &loc);
	std::string ret;

	if (it != loc.end())
	{
		// std::cout << "PASS REF DIR IF\n";

		std::string ref_trim_loc = trim_loc_ref(this->_referer, it->get_path());

		std::string tmp;
		if (it2 != loc.end() && it->get_path() == it2->get_path())
		{
			if (this->_referer.back() != '/' && !is_url_dir(this->_base_url))
			{
				tmp += ref_trim_loc;
				tmp.insert(tmp.end(), this->_base_url.begin() + this->_base_url.find_last_of('/'), this->_base_url.end());
			}
			else
				tmp.assign(this->_base_url.begin() + it2->get_path().size(), this->_base_url.end());
		}
		else
			tmp = this->_base_url;
		ret += "/" + it->get_root();
		if (it->get_root().back() != '/')
			ret += "/";
		ret += tmp;
		if (this->is_url_dir(this->_base_url) == true)
			ret += "/" +  it->get_index();
		// std::cout << BLUE << tmp << NC << '\n';
	}
	else
	{
		// std::cout << "PASS REF DIR ELSE\n";

		ret +=  "/" + this->_block->get_default_root() + this->_base_url;
		if (this->is_url_dir(this->_base_url) == true)
			ret += "/" + this->_block->get_default_path();
	}
	this->_url += ret;
}

void		Request::treatUrl_with_full_referer()
{
	std::vector<loc_block> loc = this->_block->get_location();
	std::vector<loc_block>::iterator it = location_found(this->_referer, &loc);
	std::vector<loc_block>::iterator it2 = location_found(this->_base_url, &loc);
	std::string ret;

	if (it != loc.end())
	{
		// std::cout << "PASS REF FULL IF\n";

		std::string tmp;
		if (it2 != loc.end())
			tmp.assign(this->_base_url.begin() + it2->get_path().size(), this->_base_url.end());
		else
			tmp = this->_base_url;
		ret += "/" + it->get_root();
		if (it->get_root().back() != '/')
			ret += "/";
		ret += tmp;
	}
	else
	{
		// std::cout << "PASS REF FULL ELSE\n";

		ret +=  "/" + this->_block->get_default_root() + this->_base_url;
	}
	this->_url += ret;
}

bool		error_tag_found(std::string url)
{
	std::string tmp_url(url.begin(), url.begin() + url.find_last_of('/'));

	if (tmp_url.front() == '/')
		tmp_url.erase(tmp_url.begin());

	if (tmp_url.empty() == false)
	{
		if (tmp_url == "$ERROR$")
			return (true);
	}
	return (false);
}

bool		is_url_php(std::string url)
{
	std::string tmp_url(url.begin() + url.find_last_of('/'), url.end());

	if (tmp_url.find_first_of('.') != tmp_url.npos)
	{
		tmp_url.erase(tmp_url.begin(), tmp_url.begin() + tmp_url.find_first_of('.'));
		if (tmp_url.compare(".php") == 0)
			return (true);
	}
	return (false);
}

void		Request::treatUrl_with_err_referer(const std::string ref_code)
{
	// std::cout << "ERR REFERER PROCESSING ! " << ref_code << "\n";

	std::map<std::string, std::string> error_page_stocked = this->_block->get_error_page();
	std::map<std::string, std::string>::iterator	it = error_page_stocked.find(ref_code);

	std::string error_page;

	if (it != error_page_stocked.end())
		error_page = it->second;
	else if (ref_code[0] == '4')
		error_page = error_page_stocked.find("4xx")->second;
	else if (ref_code[0] == '5')
		error_page = error_page_stocked.find("5xx")->second;

	std::string path(error_page.begin(), error_page.begin() + error_page.find_last_of('/'));
	std::string file;

	if (is_url_php(this->_base_url) == true)
	{
		this->_url = error_page;
		return;
	}
	if (error_tag_found(this->_base_url) == true)
		file.assign(this->_base_url.begin() + this->_base_url.find_last_of('/'), this->_base_url.end());
	else
	{
		std::vector<loc_block> loc = this->_block->get_location();
		std::vector<loc_block>::iterator it = location_found(this->_base_url, &loc);
		std::string url_file;

		if (it != loc.end())
			url_file.assign(this->_base_url.begin() + it->get_path().size(), this->_base_url.end());
		else
			url_file = this->_base_url;
		file = "/" + url_file;
	}
	this->_url = path + file;
	this->_response_status_code = 200;
}
