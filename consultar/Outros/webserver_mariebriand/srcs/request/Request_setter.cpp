/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_setter.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 02:56:31 by vmoreau           #+#    #+#             */
/*   Updated: 2022/02/18 11:25:03 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

/*	All setters (one for each attribute corresponding to a field of the HTTP _request):
	(The following three assign the value of the second element of the pair returned by std::map::find(),
	which corresponds to the mapped-value of the std::map this->info).
*/
void				Request::setMethod(std::string& line)
{
	this->_method = this->extractInfo(line);

	std::string valid[] = {"GET", "POST", "DELETE", "HEAD", "PUT", "CONNECT", "OPTIONS", "TRACE", "PATCH"};
	bool is_valid = false;
	for (size_t i = 0; i < 9; i++)
	{
		if (this->_method.compare(valid[i]) == 0)
			is_valid = true;
	}
	if (is_valid == false)
		this->_response_status_code = 400;
}

void				Request::setUrl(std::string& line)
{
	this->_base_url = this->extractInfo(line);

	if (this->_base_url.size() > 512) // In order to have a small url to pass to provoque the error.
		this->_response_status_code = 414;
}

void				Request::setProtocolVersion(std::string& line)
{
	this->_protocol_version = this->extractInfo(line);
	if (this->_protocol_version != "HTTP/1.1")
		this->_response_status_code = 505;
	if (this->_protocol_version.find_first_of('\r') == this->_protocol_version.npos)
		return ;
}

void				Request::setHost()
{
	std::map<std::string, std::string>::iterator	it = this->_stock.find("Host:");
	if (it != this->_stock.end())
	{
		this->_host = it->second;
		if (this->_host.find_first_of('\r') != this->_host.npos)
		{
			this->_host.clear();
			this->_host.assign(it->second.begin(), it->second.begin() + it->second.find_first_of('\r'));
		}
	}
	else
		this->_response_status_code = 400;
}

void				Request::setUserAgent()
{
	std::map<std::string, std::string>::iterator	it = this->_stock.find("User-Agent:");
	if (it != this->_stock.end())
	{
		this->_user_agent = it->second;
		if (this->_user_agent.find_first_of('\r') != this->_user_agent.npos)
		{
			this->_user_agent.clear();
			this->_user_agent.assign(it->second.begin(), it->second.begin() + it->second.find_first_of('\r'));
		}
	}
}

void				Request::setAccept()
{
	std::map<std::string, std::string>::iterator	it = this->_stock.find("Accept:");
	if (it != this->_stock.end())
	{
		this->_accept = it->second;
		if (this->_accept.find_first_of('\r') != this->_accept.npos)
		{
			this->_accept.clear();
			this->_accept.assign(it->second.begin(), it->second.begin() + it->second.find_first_of('\r'));
		}
	}
}

void				Request::setAcceptLanguage()
{
	std::map<std::string, std::string>::iterator	it = this->_stock.find("Accept-Language:");
	if (it != this->_stock.end())
	{
		this->_accept_language = it->second;
		if (this->_accept_language.find_first_of('\r') != this->_accept_language.npos)
		{
			this->_accept_language.clear();
			this->_accept_language.assign(it->second.begin(), it->second.begin() + it->second.find_first_of('\r'));
		}
	}
}

void				Request::setAcceptEncoding()
{
	std::map<std::string, std::string>::iterator	it = this->_stock.find("Accept-Encoding:");
	if (it != this->_stock.end())
	{
		this->_accept_encoding = it->second;
		if (this->_accept_encoding.find_first_of('\r') != this->_accept_encoding.npos)
		{
			this->_accept_encoding.clear();
			this->_accept_encoding.assign(it->second.begin(), it->second.begin() + it->second.find_first_of('\r'));
		}
	}
}

void				Request::setConnection()
{
	std::map<std::string, std::string>::iterator	it = this->_stock.find("Connection:");
	if (it != this->_stock.end())
	{
		this->_connection = it->second;
		if (this->_connection.find_first_of('\r') != this->_connection.npos)
		{
			this->_connection.clear();
			this->_connection.assign(it->second.begin(), it->second.begin() + it->second.find_first_of('\r'));
		}
	}
}

void				Request::setReferer()
{
	std::map<std::string, std::string>::iterator	it = this->_stock.find("Referer:");
	if (it != this->_stock.end())
	{
		this->_referer = it->second;
		if (this->_referer.find_first_of('\r') != this->_referer.npos)
		{
			this->_referer.clear();
			this->_referer.assign(it->second.begin(), it->second.begin() + it->second.find_first_of('\r'));
		}
		this->_referer = this->treat_referer(this->_referer);
	}
}

bool				comp_line(std::vector<unsigned char> line1, std::vector<unsigned char> line2)
{
	for (size_t i = 0; i < line1.size(); i++)
	{
		if (line1[i] != line2[i])
			return (false);
	}
	return (true);
}

size_t				Request::find_pos(std::string line_to_find)
{
	std::vector<unsigned char> to_find;
	for (size_t i = 0; i < line_to_find.size(); i++)
		to_find.push_back(line_to_find[i]);

	for (size_t i = 0; i < this->_vec_request.size(); i++)
	{
		// std::cout << this->_vec_request[i] << '\n';
		if (this->_vec_request[i] == to_find[0])
		{
			std::vector<unsigned char> line_to_test;
			size_t j = i;
			while (this->_vec_request[j] != '\r' && this->_vec_request[j] != '\n')
			{
				line_to_test.push_back(this->_vec_request[j]);
				j++;
			}
			if (comp_line(to_find, line_to_test) == true)
				return (i);
		}
	}
	// std::cout << '\n';
	return (-1);
}

void				Request::setBody(std::string& full_resp)
{

	size_t pos = full_resp.find("------WebKitFormBoundary");
	if (pos != std::string::npos)
	{
		std::string	tmp(full_resp.substr(pos + 1, full_resp.size() - (pos + 1)));
		this->_body = tmp;

		size_t pos2 = find_pos("------WebKitFormBoundary");
		if (pos2 > 0)
		{
			this->_vec_request.erase(this->_vec_request.begin(), this->_vec_request.begin() + pos2);
			for (std::vector<unsigned char>::iterator it = this->_vec_request.begin(); it != this->_vec_request.end(); it++)
				this->_vec_body.push_back(*it);
		}
		else
		{
			for (std::string::iterator it = this->_body.begin(); it != this->_body.end(); it++)
				this->_vec_body.push_back(*it);
		}
	}
	else
	{
		pos = full_resp.find_last_of("\n");
		std::string	tmp(full_resp.substr(pos + 1, full_resp.size() - (pos + 1)));
		this->_body = tmp;
	}

	// std::cout << this->_body.size() << '\n';
	if (this->_body.size() > (unsigned long)this->_block->get_client_max_body_size() && (this->_referer.size() == 0 || this->_method == "POST"))
		this->_response_status_code = 413;

}

void				Request::set_queryString()
{
	if (this->_method == "GET")
	{	size_t	i;

		i = this->_base_url.find_first_of('?');
		if (i != std::string::npos)
			this->_queryString.assign(this->_base_url, i + 1, std::string::npos);
	}
	else if (this->_method == "POST" && this->_body.find("------WebKitFormBoundary") == this->_body.npos)
		this->_queryString = this->_body;
	else
		this->_queryString = "";
}

void				Request::set_contentLength()
{
	std::map<std::string, std::string>::iterator	it = this->_stock.find("Content-Length:");
	if (it != this->_stock.end())
	{
		this->_content_length = it->second;
		if (this->_content_length.find_first_of('\r') != this->_content_length.npos)
		{
			this->_content_length.clear();
			this->_content_length.assign(it->second.begin(), it->second.begin() + it->second.find_first_of('\r'));
		}
	}
}

void				Request::set_contentType()
{
	std::map<std::string, std::string>::iterator	it = this->_stock.find("Content-Type:");
	if (it != this->_stock.end())
	{
		this->_content_type = it->second;
		if (this->_content_type.find_first_of('\r') != this->_content_type.npos)
		{
			this->_content_type.clear();
			this->_content_type.assign(it->second.begin(), it->second.begin() + it->second.find_first_of('\r'));
		}
	}
	else
		this->_content_type = "";
}

void		Request::setError(int code)
{
	this->_response_status_code = code;
	std::stringstream	out;
	std::string			str;

	out << this->_response_status_code;
	str = out.str();

	std::map<std::string, std::string> error_page_stocked = this->_block->get_error_page();
	std::map<std::string, std::string>::iterator	it = error_page_stocked.find(str);

	if (it != error_page_stocked.end())
		this->_response_url = it->second;
	else if (str[0] == '4')
		this->_response_url = error_page_stocked.find("4xx")->second;
	else if (str[0] == '5')
		this->_response_url = error_page_stocked.find("5xx")->second;
}

void		Request::set_error_recv()
{
	this->setError(500);
	this->_response_protocol_version = "HTTP/1.1";
	this->_url = this->_response_url;
	this->_connexion_end = true;
}
