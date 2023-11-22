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

void	Utils::trim( std::string & line )
{
	short unsigned int	i;

	i = 0;
	if (line.size())
	{
//		while (line[i] && std::isspace(line[i]))
		while (i < line.size() && std::isspace(line[i]))
			i++;
		line.erase(0, i);
		if (line.size())
		{
			i = (line.size() - 1);
			while (i != 0 && std::isspace(line[i]))
				i--;
			i++;
			line.erase(i , (line.size() - i));
		}
	}
}

std::vector<std::string> Utils::split( const std::string line, std::string sep )
{
//	std::cout << "line in split: " << line << std::endl;
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
/*
		end = 0;
		while (end < str.size())
		{
			std::cout << "split: " << str[end] << std::endl;
			end++;
		}
*/
	}
	return (str);
}

int	Utils::atoi(const std::string line)
{
	int		signal;
	int		nbr;
	size_t	i;
//	std::cout << "line in atoi: string " << line << std::endl;
	signal = 1;
	i = 0;
	while ( (i < line.size()) && (std::isspace(line[i])) )
		i++;
	if ((line[i] == '+') || (line[i] == '-'))
	{
		if (line[i] == '-')
			signal = -1;
		i++;
	}
	nbr = 0;
	while ((i < line.size()) && std::isdigit(line[i]))
	{
		nbr = (10 * nbr) + (line[i] - '0');
		i++;
	}
//	std::cout << "line in atoi: int    " << (nbr * signal) << std::endl;
	return (nbr * signal);
}
