/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:23:54 by woliveir          #+#    #+#             */
/*   Updated: 2022/05/09 15:09:49 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

Utils::Utils( void ) {}

Utils	&Utils::operator=( const Utils &src )
{
	if (this != &src)
		return (*this);
	else
		return (*this);
}

Utils::Utils( const Utils& copy )
{
	*this = copy;
	return ;
}

Utils::~Utils( void ) {}

void	Utils::ltrim(std::string & line, std::string c)
{
	short unsigned int	i;

	i = 0;
	if (line.size())
	{
		while (i < line.size() && line[i] == c[0])
			i++;
		line.erase(0, i);
	}
}

void	Utils::rtrim(std::string & line, std::string c)
{
	short int	i;

	i = 0;
	if (line.size())
	{
		i = (line.size() - 1);
		while (i != 0 && line[i] == c[0])
			i--;
		if ((i == 0) && (line[i] == c[0]))
			i--;
		i++;
		line.erase(i , (line.size() - i));
	}
}


void	Utils::trim(std::string & line, std::string c)
{
	if (line.size())
	{
		Utils::ltrim(line, c);
		Utils::rtrim(line, c);
	}
}

std::vector<std::string>	Utils::split( const std::string line, std::string sep )
{
	std::vector<std::string>	str;
	size_t						start;
	size_t						end;

	start = 0;
	end = 0;
	if (line.size())
	{
		while (end < line.size())
		{
			end = line.find_first_of(sep, start);
			str.push_back(line.substr(start, (end - start)));
			start = line.find_first_not_of(sep, end);
		}
	}
	return (str);
}

int	Utils::atoi(const std::string line)
{
	int		signal;
	int		nbr;
	size_t	i;

	signal = 1;
	i = 0;
	nbr = 0;
	while ((i < line.size()) && (std::isspace(line[i])))
		i++;
	if ((line[i] == '+') || (line[i] == '-'))
	{
		if (line[i] == '-')
			signal = -1;
		i++;
	}
	while ((i < line.size()) && std::isdigit(line[i]))
	{
		nbr = (10 * nbr) + (line[i] - '0');
		i++;
	}
	return (nbr * signal);
}

short int	Utils::getTypePath(const std::string & path)
{
	struct stat	buffer;
	short int	ret;

	ret = stat(path.c_str(), &buffer);
	if (ret == 0)
	{
		if (S_ISREG(buffer.st_mode))
			return (1);
		else if (S_ISDIR(buffer.st_mode))
			return (2);
		else if (S_ISSOCK(buffer.st_mode))
			return (3);
		else
			return (4);
	}
	else
		return (-1);
}

short int	Utils::checkFile(const std::string & path, short int mode)
{
	return (access(path.c_str(), mode));
}

bool	Utils::isFileExistAndReadable(const std::string & path, const std::string & file)
{
	if ((Utils::getTypePath(file) == 1) && (Utils::checkFile(file, R_OK) == 0))
		return (true);
	if ((Utils::getTypePath(path + file) == 1) && (Utils::checkFile(path + file, R_OK) == 0))
		return (true);
	return (false);
}
