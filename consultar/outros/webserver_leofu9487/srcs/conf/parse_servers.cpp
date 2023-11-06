/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_servers.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 13:13:08 by xli               #+#    #+#             */
/*   Updated: 2021/11/30 15:11:43 by yfu              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "conf.hpp"
#include "ServerInfo.hpp"

// fill the vector result
// if any error, throw an exception (check the file includes/exception.hpp)

void parse_servers(std::vector<ServerInfo> &result, char *conf_file_path)
{
	if (!conf_file_path)
		throw(InvalidConfFilePath());
	std::ifstream	file;
	file.open(conf_file_path);
	if (!file)
		throw(InvalidConfFile());
	char	c;
	std::string	str;
	while (file.get(c))
		str.push_back(c);
	file.close();
	if (valid_bracket(str) == false)
		throw(ConfFileParseError("invalid numbers bracket"));
	int i = 0;
	while (i < nb_lines(str))
	{
		if (!is_skippable(str, i))
		{
			std::string	line = get_line(str, i);
			if (!line.compare(0, 8, "server {"))
			{
				if (nb_tokens(line.c_str()) != 2 || line.compare(line.size() - 1, 1, "{"))
					throw(ConfFileParseError("invalid server header"));
				new_server(result, str, i);
			}
		}
		i++;
	}
}

/*
** Parsing and filling server info
*/

void new_server(std::vector<ServerInfo> &result, std::string &str, int &pos)
{
	ServerInfo	new_server;
	int	ct = pos + 1;
	while (ct < closing_bracket(str, pos))
	{
		std::string line = get_line(str, ct);
		std::string attri = line.substr(0, line.find(" "));
		if (is_valid_attribute(attri, server_attributes) || is_skippable(str, ct))
		{
			if (!line.compare(0, 7, "listen "))
				new_server.set_server(PORT, 7, line);
			else if (!line.compare(0, 12, "server_name "))
				new_server.set_server(NAME, 12, line);
			else if (!line.compare(0, 11, "error_page "))
				new_server.set_server(ERROR, 11, line);
			else if (!line.compare(0, 9, "max_size "))
				new_server.set_server(SIZE, 9, line);
			else if (!line.compare(0, 9, "location "))
			{
				if (nb_tokens(line.c_str()) != 3 || line.compare(line.size() - 1, 1, "{"))
					throw(ConfFileParseError("invalid location header"));
				new_location(new_server, str, ct);
			}
		}
		else
			throw(ConfFileParseError("wrong input in server"));
		ct++;
	}

	result.push_back(new_server);
	// new_server.print();
}

/*
** Parsing and filling location info
*/

void new_location(ServerInfo &n_server, std::string &str, int &ct)
{
	Location	n_location(n_server.get_port());
	while (get_line(str, ct) != "}")
	{
		if (!is_skippable(str, ct))
		{
			std::string	line = get_line(str, ct);
			if (!line.compare(0, 9, "location "))
			{
				line.erase(line.size() - 1, 1);
				n_location.set_location(URI, 9, line);
			}
			else if (!line.compare(0, 10, "autoindex "))
				n_location.set_location(AUTOINDEX, 10, line);
			else if (!line.compare(0, 6, "index "))
				n_location.set_location(INDEX, 6, line);
			else if (!line.compare(0, 5, "root "))
				n_location.set_location(ROOT, 5, line);
			else if (!line.compare(0, 9, "redirect "))
				n_location.set_location(REDIRECT, 9, line);
			else if (!line.compare(0, 13, "allow_method "))
				n_location.set_location(METHOD, 13, line);
			else if (!line.compare(0, 12, "upload_path "))
				n_location.set_location(UPLOADPATH, 12, line);
			else if (!line.compare(0, 4, "cgi "))
				n_location.set_location(CGI, 4, line);
			else
				throw(ConfFileParseError("wrong input in location"));
		}
		ct++;
	}
	n_server.add_location(n_location);
	// n_location.print();
}

/*
** Varify if bracket are alway in pairs
*/

bool valid_bracket(std::string str)
{
	std::stack<char>	bracket;
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == '{')
			bracket.push(str[i]);
		if (str[i] == '}')
		{
			if (bracket.empty() || bracket.top() != '{')
				return false;
			if (!bracket.empty() && bracket.top() == '{')
				bracket.pop();
		}
	}
	if (bracket.empty())
		return true;
	return false;
}

/*
** Get the line of the matching closing bracket
*/

int closing_bracket(std::string str, int pos)
{
	int	line;
	line = pos + 1;
	int	ct = 0;
	for (int i = pos; i < nb_lines(str); i++)
	{
		if (get_line(str, i).find("{") != std::string::npos)

			++ct;
		else if (get_line(str, i).find("}") != std::string::npos)
		{
			--ct;
			if (ct == 0)
				return (line);
		}
		++line;
	}
	return (-1);
}

/*
** Get total number of lines of the string
*/

int nb_lines(std::string str)
{
	int	i;
	int	lines;

	i = 0;
	lines = 1;
	while (str[i])
	{
		if (str[i++] == '\n')
			++lines;
	}
	return lines;
}

/*
** Get n-th line of string
*/

std::string get_line(std::string str, int n)
{
	int	i;
	int	j;
	int	ct;

	if (n >= nb_lines(str))
		return std::string();
	i = 0;
	ct = 0;
	while (ct < n)
	{
		if (str[i++] == '\n')
			++ct;
	}
	while (std::isspace(str[i]) && str[i] != '\n')
		++i;
	j = 0;
	while (str[i + j] && str[i + j] != '\n')
		++j;
	while (j > 0 && std::isspace(str[i + j - 1]))
		--j;
	return (std::string(str, i, j));
}

/*
** Get numbers of tokens of string
*/

int nb_tokens(const char *str)
{
	int ct = 0;
	int res = 0;
	while (str[ct] != '\0')
	{
		while (isspace(str[ct]))
			ct++;
		int tmp = ct;
		while (isalnum(str[ct]) || ispunct(str[ct]))
			ct++;
		if (tmp != ct)
			res++;
	}
	return res;
}

/*
** Check if the attribute is valid
*/

bool is_valid_attribute(std::string str, const char *valid_names[])
{
	size_t i;

	i = 0;
	while (valid_names[i])
	{
		if (str == valid_names[i])
			return true;
		++i;
	}
	return false;
}

/*
** Check if the line is skippable
*/

bool is_skippable(std::string str, int pos)
{
	std::string line;

	line = get_line(str, pos);
	return (line.size() == 0 || line[0] == '#' || line[0] == '}');
}
