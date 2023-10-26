/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loc_block.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 14:44:18 by vmoreau           #+#    #+#             */
/*   Updated: 2022/02/02 16:43:25 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "loc_block.hpp"

bool check_method(std::string method)
{
	std::string valid[] = {"GET", "POST", "DELETE", "HEAD", "PUT", "CONNECT", "OPTIONS", "TRACE", "PATCH"};

	for (size_t i = 0; i < 9; i++)
	{
		if (method.compare(valid[i]) == 0)
			return (true);
	}
	return (false);
}

loc_block::loc_block(/* args */)
{
}

loc_block::~loc_block()
{
}

loc_block& loc_block::operator=(const loc_block cpy)
{
	this->_block = cpy._block;
	this->_path_conf = cpy._path_conf;
	this->_path = cpy._path;
	this->_root = cpy._root;
	this->_method_limit = cpy._method_limit;
	this->_index = cpy._index;

	return (*this);
}

// ********************* PARSING ********************* //

void loc_block::pars_loc(std::vector<std::string> block, std::string path)
{
	this->_block = block;
	this->_path_conf = path;
	size_t i = 2;
	std::vector<std::string> tmp(this->_block);

	while (i < tmp.size() - 1)									// Pars all info in location block
	{
		if (tmp[i].find_first_of(' ') == tmp[i].npos)
			throw ConfFile(" Server block is wrongly formatted");
		std::string key(tmp[i].begin(), tmp[i].begin() + tmp[i].find_first_of(' '));
		std::string value(tmp[i].begin() + key.size(), tmp[i].end());

		if (key.compare("path") == 0)
			this->set_path(value);
		else if (key.compare("root") == 0)
			this->set_root(value);
		else if (key.compare("method_limit") == 0)
			this->set_method_limit(value);
		else if (key.compare("index") == 0)
			this->set_index(value);

		i++;
	}

	if (this->_path.empty() == true)
		throw NoPathFound(this->_path_conf);
	if (this->_root.empty() == true)
		throw NoRootFound(this->_path_conf);
	if (this->_index.empty() == true)
		throw NoIndexFound(this->_path_conf);
	if (this->_method_limit.empty() == true)
	{
		this->_method_limit.push_back("GET");
		this->_method_limit.push_back("POST");
		this->_method_limit.push_back("DELETE");
		this->_method_limit.push_back("HEAD");

		std::cout << YELLOW << "Warning: " << NC << " Method_limit is missing in ";
		std::cout << this->_path_conf << " method_limit is set by default at GET, POST, DELETE, HEAD" << '\n';
	}
}

// ********************* SETTER ********************** //

void loc_block::set_path(std::string value)
{
	std::string path(value.begin() + value.find_first_not_of(' '), value.end());

	this->_path = path;
}

void loc_block::set_root(std::string value)
{
	std::string root(value.begin() + value.find_first_not_of(' '), value.end());

	this->_root = root;
}

void loc_block::set_method_limit(std::string value)
{
	std::string m_l(value.begin() + value.find_first_not_of(' '), value.end());

	int last_space_pos = -1;

	for (size_t i = 0; i < m_l.size(); i++)
	{
		if (m_l[i] == ' ' || m_l[i] == ',')
		{
			std::string tmp(m_l.begin() + last_space_pos + 1, m_l.begin() + i);
			this->_method_limit.push_back(tmp);
			while (m_l[i + 1] == ' ' || m_l[i + 1] == ',')
				i++;
			last_space_pos = i;
		}
		if (i + 1 == m_l.size())
		{
			std::string tmp(m_l.begin() + last_space_pos + 1, m_l.begin() + i + 1);
			this->_method_limit.push_back(tmp);
		}
	}
	for (size_t i = 0; i < this->_method_limit.size(); i++)
	{
		if (check_method(this->_method_limit[i]) == false)
			throw ConfFile(" method \"" + this->_method_limit[i] + "\" is not a valid method");
	}
}

void loc_block::set_index(std::string value)
{
	std::string index(value.begin() + value.find_first_not_of(' '), value.end());

	this->_index = index;
}

