/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   confpars.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 16:02:13 by vmoreau           #+#    #+#             */
/*   Updated: 2022/02/16 22:21:51 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "confpars.hpp"
#include <wctype.h>

// ********************** UTILS ********************** //

std::string norm_line(std::string line)
{
	std::string ret(line);
	for (size_t i = 0; i < ret.size(); i++)
	{
		if (ret[i] == '\t')
			ret[i] = ' ';
	}
	int j = ret.find_first_not_of(' ');
	std::string ret2(ret.begin() + j, ret.end());

	for (std::string::reverse_iterator rit = ret2.rbegin(); rit != ret2.rend(); rit++)
	{
		if (*rit != ' ' && *rit != ';')
			break;
		ret2.pop_back();
	}
	return (ret2);
}

bool is_emptyline_commentline(std::string line)
{
	int i = 0;

	while (iswspace(line[i]))
		i++;
	if (line[i] == '\0' || line[i] == '#')
		return (true);
	else
		return (false);
}

// ******************* CONSTRUCTOR ******************* //

confpars::confpars()
{
	// std::cout << "Default constructor call\n";
}

confpars::~confpars()
{
}

// confpars& confpars::operator=(const confpars conf)
// {
// 	this->_error_page = conf._error_page;
// 	this->_fc = conf._fc;
// 	this->_file = conf._file;
// 	this->_path = conf._path;
// 	this->_server = conf._server;
// 	this->_server_block = conf._server_block;
// }

// ******************** FD MANIP ********************* //
void confpars::open_fc(const char *path)
{
	if (path != NULL)
	{
		this->_path = path;
		this->_fc.open(this->_path.c_str());
		if (this->_fc.is_open() == false)
			throw FileNotFound(this->_path);
	}
	else
		FileNotFound("NULL");
}

void confpars::reopen_fc()
{
	this->_fc.close();
	this->open_fc(NULL);
}

void confpars::close_fc()
{
	this->_fc.close();
}


// ********************* SETTER ********************** //


void confpars::set_error_page(std::string value)
{
	std::string code_err(value.begin() + value.find_first_not_of(" "), value.begin() + value.find_first_not_of(" ") + 4);
	std::string path_err_tmp(value.begin() + code_err.size(), value.end());
	std::string path_err(path_err_tmp.begin() + path_err_tmp.find_first_not_of(" "), path_err_tmp.end());

	if (code_err.back() != ' ')
		throw ConfFile(" Error_page code is based on 5xx or 4xx only");
	if (code_err.front() != '4' && code_err.front() != '5')
		throw ConfFile(" Error_page code is based on 5xx or 4xx only");

	code_err.pop_back();

	for (size_t i = 0; i < code_err.size(); i++)
		if (!isdigit(code_err[i]))
			if (code_err[i] != 'x')
				throw ConfFile(" CodeError is not digit");

	this->_error_page.insert(std::make_pair(code_err, path_err));
}

void confpars::set_cgi_path(std::string value)
{
	std::string val(value.begin() + value.find_first_not_of(" "), value.end());

	this->_cgi_path = val;
}

// ******************** PARSING ********************** //

void confpars::pars_fc(std::ifstream &fc)
{
	std::string line;

	while(std::getline(fc, line))
	{
		if (!is_emptyline_commentline(line))
			this->_file.push_back(norm_line(line));
	}
	this->reopen_fc();

	this->pars_http();											// Pars all information which not in server or location block
	for (size_t i = 0; i < this->_server_block.size(); i++)		// Loop on all server block and pars them
	{
		serv_block tmp;

		tmp.pars_serv(this->_server_block[i], this->_path, this->_error_page);
		this->_server.push_back(tmp);
	}
	if (this->server_port_unique() == false)
		throw ConfFile("Servers Port Duplicate in " + this->_path);
}

void confpars::pars_http()
{
	std::vector<std::string> tmp(this->_file);
	size_t i = 0;
	size_t nb_serv = 0;

	for (size_t j = 0; j < this->_file.size(); j++)
		if (this->_file[j].compare("server") == 0)
			nb_serv++;

	if (nb_serv == 0)
		throw NoServerFound(this->_path);
	if (tmp[i++].compare("http") != 0)
		throw ConfFile(this->_path + " Must start by http block\n");
	if (tmp[i++].compare("{") != 0)
		throw ConfFile(this->_path + " HTTP block \"{\" Missing\n");
	tmp = this->separate_server_block(tmp, nb_serv);

	while (i < tmp.size() - 1)
	{
		if (tmp[i].find_first_of(' ') == tmp[i].npos)
			throw ConfFile(" HTTP block is wrongly formatted");
		std::string key(tmp[i].begin(), tmp[i].begin() + tmp[i].find_first_of(' '));
		std::string value(tmp[i].begin() + key.size(), tmp[i].end());

		if (key.compare("error_page") == 0)
			this->set_error_page(value);
		else if (key.compare("cgi_path") == 0)
			this->set_cgi_path(value);
		i++;
	}
	if (this->_error_page.empty() == true)
	{
		std::cout << YELLOW << "Warning: " << NC << " Error pages is missing in ";
		std::cout << this->_path << " Error pages are set by default at :\n\t" << PURPLE << DEFAULT_ERR_400 << "\n\t" << DEFAULT_ERR_404 << "\n\t" << DEFAULT_ERR_4xx << "\n\t" << DEFAULT_ERR_500 << "\n\t" << DEFAULT_ERR_5xx << '\n' << NC;
		this->_error_page.insert(std::make_pair("400", DEFAULT_ERR_400));
		this->_error_page.insert(std::make_pair("404", DEFAULT_ERR_404));
		this->_error_page.insert(std::make_pair("4xx", DEFAULT_ERR_4xx));
		this->_error_page.insert(std::make_pair("500", DEFAULT_ERR_500));
		this->_error_page.insert(std::make_pair("5xx", DEFAULT_ERR_5xx));
	}
	else
		this->check_minimal_error_page();
	if (this->_cgi_path.empty() == true)
		throw ConfFile("No cgi_path found in " + this->_path);

	this->is_path_error_page_ok();

	// DEBUG //
	// for (std::map<std::string, std::string>::iterator it = this->_error_page.begin(); it != this->_error_page.end(); it++)
	// 	std::cout << it->first << "-> \"" << it->second << "\"\n";
	// std::cout << this->_cgi_path << '\n';
}

// ************** INTERNE FUNCTION PARS ************** //

std::vector<std::string> confpars::save_server_block(std::vector<std::string> tmp, size_t *pos)
{
	size_t i = *pos;
	std::vector<std::string> ret;
	int bracket = 0;

	ret.push_back(tmp[i++]);
	if (tmp[i].compare("{") != 0)
		throw ConfFile(this->_path + " Server block \"{\" Missing\n");
	bracket++;
	ret.push_back(tmp[i++]);
	while (i < tmp.size() && bracket != 0)
	{
		if (tmp[i].compare("server") == 0)
			throw ConfFile(this->_path + " Server block \"}\" Missing\n");
		if (tmp[i].compare("{") == 0)
			bracket++;
		if (tmp[i].compare("}") == 0)
			bracket--;
		ret.push_back(tmp[i]);
		i++;
	}
	if (i != tmp.size() && (tmp[i].compare("server") != 0 && tmp[i].compare("}") != 0))
		throw ConfFile(this->_path + " Server block \"{\" Missing\n");
	*pos = i;
	return (ret);
}

std::vector<std::string> confpars::separate_server_block(std::vector<std::string> tmp, size_t nb_serv)
{
	std::vector<std::string>ret;

	size_t i = 0;

	while (nb_serv > 0)
	{
		while (i < tmp.size() && tmp[i].compare("server"))
		{
			ret.push_back(tmp[i]);
			i++;
		}
		this->_server_block.push_back(this->save_server_block(tmp , &i));
		nb_serv--;
	}
	if (i == tmp.size() || (tmp[i].compare("}") != 0))
		throw ConfFile(this->_path + " HTTP or last Server block \"}\" Missing\n");
	ret.push_back(tmp[i++]);
	if (i != tmp.size())
		throw ConfFile(this->_path + " Server block \"{\" Missing\n");

	return (ret);
}

bool confpars::server_port_unique()
{
	for (std::vector<serv_block>::iterator it = this->_server.begin(); it != this->_server.end(); it++)
	{
		for (std::vector<serv_block>::iterator it2 = it + 1; it2 != this->_server.end(); it2++)
		{
			if (it->get_port() == it2->get_port() && it->get_host() == it2->get_host())
				return (false);
		}
	}
	return (true);
}

void		confpars::check_minimal_error_page()
{
	if (this->_error_page.find("400") == this->_error_page.end())
	{
		std::cout << PURPLE << "\tError page for 400 is missing, set by default to " << DEFAULT_ERR_400 << NC << '\n';
		this->_error_page.insert(std::make_pair("400", DEFAULT_ERR_400));
	}
	if (this->_error_page.find("404") == this->_error_page.end())
	{
		std::cout << PURPLE << "\tError page for 404 is missing, set by default to " << DEFAULT_ERR_404 << NC << '\n';
		this->_error_page.insert(std::make_pair("404", DEFAULT_ERR_404));
	}
	if (this->_error_page.find("4xx") == this->_error_page.end())
	{
		std::cout << PURPLE << "\tError page for 4xx is missing, set by default to " << DEFAULT_ERR_4xx << NC << '\n';
		this->_error_page.insert(std::make_pair("4xx", DEFAULT_ERR_4xx));
	}
	if (this->_error_page.find("500") == this->_error_page.end())
	{
		std::cout << PURPLE << "\tError page for 500 is missing, set by default to " << DEFAULT_ERR_500 << NC << '\n';
		this->_error_page.insert(std::make_pair("500", DEFAULT_ERR_500));
	}
	if (this->_error_page.find("5xx") == this->_error_page.end())
	{
		std::cout << PURPLE << "\tError page for 5xx is missing, set by default to " << DEFAULT_ERR_5xx << NC << '\n';
		this->_error_page.insert(std::make_pair("5xx", DEFAULT_ERR_5xx));
	}
}

bool		confpars::is_path_error_page_ok()
{
	std::string path;

	for (std::map<std::string, std::string>::iterator it = this->_error_page.begin() ; it != this->_error_page.end(); it++)
	{
		path.assign(it->second.begin(), it->second.begin() + it->second.find_last_of('/'));
		if (path.compare("./html/error") != 0)
			throw ConfFile("Error path not good formated for " + it->first + " : " + it->second);
		path.clear();
	}
	return (true);
}
