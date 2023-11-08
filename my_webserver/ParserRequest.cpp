/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:23:54 by woliveir          #+#    #+#             */
/*   Updated: 2022/05/09 15:09:49 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParserRequest.hpp"

ParserRequest::ParserRequest(void)
{}

ParserRequest	&ParserRequest::operator=(const ParserRequest & src)
{
	if (this != &src)
	{
		this->_method = src._method;
		this->_location = src._location;
		this->_requestedInf = src._requestedInf;
		this->_contentType = src._contentType;
	}
	return (*this);
}

ParserRequest::ParserRequest(const ParserRequest & copy)
{
	*this = copy;
	return ;
}

ParserRequest::~ParserRequest(void) {}

const std::string &		ParserRequest::getMethod(void) const
{
	return (this->_method);
}

const std::string &		ParserRequest::getLocation(void) const
{
	return (this->_location);
}

const std::string &		ParserRequest::getRequestedInf(void) const
{
	return (this->_requestedInf);
}

const std::string &		ParserRequest::getContentType(void) const
{
	return (this->_contentType);
}

void 	ParserRequest::parserRequest(const std::string & request)
{
//	std::cout << "start | parserRequest: " << request << std::endl;
	std::ifstream	ifs;//	temporário até utilização com a string da requisição original
	std::string		line;//	temporário até utilização com a string da requisição original
	std::string		fullRequest; //	temporário até utilização com a string da requisição original
	size_t			pos;

	ifs.open(request.c_str()); //	temporário até utilização com a string da requisição original
	if (ifs.is_open())//	temporário até utilização com a string da requisição original
	{
		while(std::getline(ifs, line))//	temporário até utilização com a string da requisição original
			fullRequest += (line + '\n');//	temporário até utilização com a string da requisição original
		ifs.close();//	temporário até utilização com a string da requisição original
	}
	pos = fullRequest.find(" HTTP/");//	substituir fullRequest pela string da requisição original
	if (pos == std::string::npos)
		std::cout << "Erro na requisição" << std::endl;
	else
		this->splitRequest(fullRequest, pos);
//	std::cout << "end   | parserRequest" << std::endl;
}

void	ParserRequest::splitRequest(std::string & fullRequest, size_t & pos)
{
//	std::cout << "start | splitRequest | fullRequest: " << fullRequest << " pos: " << pos << std::endl;
	std::vector<std::string>	splitHeadRequest;
	std::vector<std::string>::iterator	i;
	size_t								j;

	splitHeadRequest = Utils::split(fullRequest.substr(0, pos), " \t\n");
	i = splitHeadRequest.begin();
	this->_method = *(i++);
	j = (*i).rfind("/");
	this->_location = (*i).substr(0, (j + 1));
	this->_requestedInf = (*i).substr((j + 1), ((*i).size() - (j + 1)));
	j = fullRequest.find("Content-Type:");
	this->_contentType = fullRequest.substr(j, (fullRequest.size() - j));
//	std::cout << "end   | splitRequest Content-Type: " << j << " contentType: " << this->_contentType << std::endl;
}
