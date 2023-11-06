/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serv_block.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 14:44:06 by vmoreau           #+#    #+#             */
/*   Updated: 2022/02/17 18:36:14 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "serv_block.hpp"


serv_block::serv_block() : _path(CONF_DEFAULT_PATH), _port(-1), _client_max_body_size(-1),  _autoindex(false)
{
	// std::cout << "Serv_Block construct\n";
}

serv_block::~serv_block()
{
}

// ********************* PARSING ********************* //

void serv_block::pars_serv(std::vector<std::string> block, std::string path, std::map<std::string, std::string> error_page)
{
	this->_block = block;
	this->_path = path;
	this->_error_page = error_page;
	std::vector<std::string> tmp(this->_block);
	size_t nb_loc = 0;
	bool is_autoindex = false;

	for (size_t i = 0; i < this->_block.size(); i++)
		if (this->_block[i].compare("location") == 0)
			nb_loc++;

	if (nb_loc == 0)
		throw NoLocationFound(this->_path);
	tmp = this->separate_loc_block(tmp, nb_loc);

	size_t i = 2;
	while (i < tmp.size() - 1)									// Pars all info in server block which not in location block
	{
		if (tmp[i].find_first_of(' ') == tmp[i].npos)
			throw ConfFile(" Server block is wrongly formatted");
		std::string key(tmp[i].begin(), tmp[i].begin() + tmp[i].find_first_of(' '));
		std::string value(tmp[i].begin() + key.size(), tmp[i].end());

		if (key.compare("listen") == 0)
			this->set_port_host(value);
		else if (key.compare("server_name") == 0)
			this->set_server_name(value);
		else if (key.compare("client_max_body_size") == 0)
			this->set_client_max_body_size(value);
		else if (key.compare("root") == 0)
			this->set_default_root(value);
		else if (key.compare("path") == 0)
			this->set_default_path(value);
		else if (key.compare("autoindex") == 0)
		{
			is_autoindex = true;
			this->set_autoindex(value);
		}

		i++;
	}

	if (this->_port == -1 && this->_host.empty() == true)
		throw NoListenFound(this->_path);
	if (this->_server_name.empty() == true)
		this->_server_name = DEFAULR_SERV_NAME;
	if (this->_client_max_body_size == -1)
	{
		this->_client_max_body_size = BODY_DEFAULT_SIZE;
		std::cout << YELLOW << "Warning: " << NC << " Client_max_body_size is missing in ";
		std::cout << this->_path << " client_max_body_size is set by default at " << BODY_DEFAULT_SIZE << '\n';
	}
	for (size_t i = 0; i < nb_loc; i++)							// Loop on all location block and pars them
	{
		loc_block tmp;

		tmp.pars_loc(this->_loc_block[i], this->_path);
		this->_location.push_back(tmp);
		// std::cout << '\n';
	}
	sort_location_block();

	if (this->_default_root.size() == 0)
	{
		this->_default_root = DEFAULT_ROOT;
		std::cout << YELLOW << "Warning: " << NC << " Default_root missing in " << this->_path << " default root set by default at " << DEFAULT_ROOT << '\n';
	}
	if (this->_default_path.size() == 0)
	{
		this->_default_path = DEFAULT_PATH;
		std::cout << YELLOW << "Warning: " << NC << " Default_path missing in " << this->_path << " default path set by default at " << DEFAULT_PATH << '\n';
	}
	if (is_autoindex == false)
	{
		this->_autoindex = false;
		std::cout << YELLOW << "Warning: " << NC << " Autoindex is missing in " << this->_path << " autoindex is set by default at off" << '\n';
	}
	// // DEBUG
	// std::cout << "Port: " << this->_port << "\n";
	// std::cout << "Host: " << this->_host << "\n";
	// std::cout << "Server name: " << this->_server_name << "\n";
	// std::cout << "Client Max Body Size: " << this->_client_max_body_size << "\n";
}

// ********************* SETTER ********************** //

void serv_block::set_port_host(std::string value)
{
	if (value.empty() == true)
		throw ConfFile(" Listen is wrongly formatted (exemple: 127.0.0.1:8080)");
	if (value.find(':') == value.npos)
	{
		std::string err_val(value.begin() + value.find_first_not_of(' '), value.end());
		throw ConfFile(" Listen \"" + err_val + "\" is wrongly formatted (exemple: 127.0.0.1:8080)");
	}
	std::string host(value.begin() + value.find_first_not_of(' '), value.begin() + value.find_first_of(':'));
	std::string port(value.begin() + value.find_first_of(':') + 1, value.end());

	if (port.size() == 0)
		throw NoPortFound(this->_path);
	for (size_t i = 0; i < port.size(); i++)
		if (!isdigit(port[i]))
			throw ConfFile(" Port \"" + port + "\" is not digit");

	if (host.compare("localhost") == 0)
	{
		host.clear();
		host = "127.0.0.1";
	}
	else
	{
		for (size_t i = 0; i < host.size(); i++)
			if (!isdigit(host[i]))
				if (host[i] != '.')
					throw ConfFile(" Host \"" + host + "\" is not digit");
	}
	check_host(host);
	this->_port = atoi(port.c_str());
	this->_host = host;

	if (this->_port < 0 || this->_port > 65535)
		throw ConfFile(" Port \"" + port + "\" is wrongly formatted. (0 >= port <= 65535)");
}

void serv_block::set_server_name(std::string value)
{
	std::string s_name(value.begin() + value.find_first_not_of(' '), value.end());

	this->_server_name = s_name;
}
void serv_block::set_client_max_body_size(std::string value)
{
	std::string c_m_b_s(value.begin() + value.find_first_not_of(' '), value.end());

	for (size_t i = 0; i < c_m_b_s.size(); i++)
		if (!isdigit(c_m_b_s[i]))
			throw ConfFile(" Client Max Body Size \"" + c_m_b_s + "\" is not digit");

	this->_client_max_body_size = atoi(c_m_b_s.c_str());
}

void serv_block::set_default_root(std::string value)
{
	std::string d_root(value.begin() + value.find_first_not_of(' '), value.end());

	this->_default_root = d_root;
}

void serv_block::set_default_path(std::string value)
{
	std::string d_path(value.begin() + value.find_first_not_of(' '), value.end());

	this->_default_path = d_path;
}

void serv_block::set_autoindex(std::string value)
{
	std::string auto_ind(value.begin() + value.find_first_not_of(' '), value.end());

	if (auto_ind.compare("on") == 0)
		this->_autoindex = true;
	else if (auto_ind.compare("off") == 0)
		this->_autoindex = false;
	else
		throw ConfFile(" Autoindex option is \"on\" or \"off\"");
}

// ************** INTERNE FUNCTION PARS ************** //

std::vector<std::string> serv_block::save_loc_block(std::vector<std::string> tmp, size_t *pos)
{
	size_t i = *pos;
	std::vector<std::string> ret;
	int bracket = 0;

	ret.push_back(tmp[i++]);
	if (tmp[i].compare("{") != 0)
		throw ConfFile(this->_path + " Location block \"{\" Missing\n");
	bracket++;
	ret.push_back(tmp[i++]);
	while (i < tmp.size() && bracket != 0)
	{
		if (tmp[i].compare("location") == 0)
			throw ConfFile(this->_path + " Location block \"}\" Missing\n");
		if (tmp[i].compare("{") == 0)
			bracket++;
		if (tmp[i].compare("}") == 0)
			bracket--;
		ret.push_back(tmp[i]);
		i++;
	}
	if (i != tmp.size() && (tmp[i].compare("location") != 0 && tmp[i].compare("}") != 0))
		throw ConfFile(this->_path + " Location block \"{\" Missing\n");
	*pos = i;
	return (ret);
}

std::vector<std::string> serv_block::separate_loc_block(std::vector<std::string> tmp, size_t nb_loc)
{
	std::vector<std::string>ret;

	size_t i = 0;

	while (nb_loc > 0)
	{
		while (i < tmp.size() && tmp[i].compare("location"))
		{
			ret.push_back(tmp[i]);
			i++;
		}
		this->_loc_block.push_back(this->save_loc_block(tmp , &i));
		nb_loc--;
	}
	if (i == tmp.size() || (tmp[i].compare("}") != 0))
		throw ConfFile(this->_path + " Server or last location block \"}\" Missing\n");
	ret.push_back(tmp[i++]);
	if (i != tmp.size())
		throw ConfFile(this->_path + " Location block \"{\" Missing\n");

	return (ret);
}

void serv_block::check_host(std::string value)
{
	std::vector<std::string> host;

	int last_dot_pos = -1;
	int nb_dot = 3;
	for (size_t i = 0; i < value.size(); i++)
	{
		if (value[i] == '.')
		{
			std::string tmp(value.begin() + last_dot_pos + 1, value.begin() + i);
			host.push_back(tmp);
			last_dot_pos = i;
			nb_dot--;
		}
		if (i + 1 == value.size())
		{
			std::string tmp(value.begin() + last_dot_pos + 1, value.begin() + i + 1);
			host.push_back(tmp);
		}
	}

	for (size_t i = 0; i < host.size(); i++)
	{
		int val = atoi(host[i].c_str());
		if (host[i].empty() == true)
			throw ConfFile( " Host \"" + value + "\" is wrongly formmated (exemple: 127.0.0.1)");
		if (i == 0)
		{
			if (val != 127)
				throw ConfFile(" Host \"" + value + "\" first byte must be 127");
		}
		else
		{
			if (val < 0 || val > 255)
				throw ConfFile(" Host \"" + value + "\" must be between 127.0.0.1 && 127.255.255.255");
		}
	}
	if (nb_dot != 0)
		throw ConfFile( " Host \"" + value + "\" is wrongly formmated (exemple: 127.0.0.1)");
}

bool comp(loc_block a, loc_block b)
{
	return (a.get_path() > b.get_path());
}

void serv_block::sort_location_block()
{
	std::sort(this->_location.begin(), this->_location.end(), comp);
}
