/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 20:57:30 by mabriand          #+#    #+#             */
/*   Updated: 2022/02/17 20:19:09 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Request.hpp"

std::string		trim_double_slash(std::string line)
{
	std::string ret;
	int i = 0;

	while (line[i] != '\0')
	{
		if (line[i] + 1 != '\0')
		{
			if (line[i] == '/')
			{
				if (line[i + 1] != '/')
					ret.push_back(line[i]);
			}
			else
				ret.push_back(line[i]);
		}
		i++;
	}
	return (ret);
}

Request::Request(int socket, serv_block *block)  :  _socket(socket), _block(block)
{
	this->_request_ready = false;
	this->_connexion_end = false;
}

Request::~Request(){}

std::string			Request::extractInfo(std::string& line) const
{
	std::string::iterator	it = line.begin();
	std::string::iterator	ite = it;

	while (std::isspace(*ite) == 0 && ite != line.end())
		ite++;
	std::string	info(it, ite);
	while (std::isspace(*ite) != 0 && ite != line.end())
		ite++;

	line.erase(it, ite);
	return (info);
}

int					Request::parse()
{
	std::vector<unsigned char> vec;
	vec.push_back('\xFF');
	vec.push_back('\xF4');
	vec.push_back('\xFF');
	vec.push_back('\xFD');
	vec.push_back('\x06');

	long int 	ret;
	std::string	buf;
	std::vector<unsigned char> buffer(BUF_SIZE + 1);
	ret = recv(this->_socket, &buffer[0], BUF_SIZE, MSG_DONTWAIT);

	// static bool test = true;			// Simulate a recv return -1;
	// if (test == true)
	// {
	// 	test = false;
	// 	this->_request_ready = true;
	// 	return (-1);
	// }

	// std::cout << "AFTER RECV: " << ret << '\n';
	if (ret >= 0)
	{
		buffer.resize(ret);
		if (ret == 5 && buffer == vec)
			this->_request_ready = true;
		buf.insert(buf.end(), buffer.begin(), buffer.begin() + ret);
		this->_request += buf;
		this->_vec_request.insert(this->_vec_request.end(), buffer.begin(), buffer.end());
		if (ret < BUF_SIZE)
			this->_request_ready = true;
	}
	if (ret < 0)
		this->_request_ready = true;
	return (ret);
}

std::string			Request::extractMapped(std::string& line) const
{
	std::string::iterator	it = line.begin();
	std::string::iterator	ite = it;

	while (ite != line.end())
		ite++;
	std::string	info(it, ite);

	line.erase(it, ite);
	return (info);
}

void				Request::buildMap(std::string& line)
{
	std::string	key = this->extractInfo(line);
	std::string	mapped = this->extractMapped(line);

	std::pair<std::string, std::string> elem(key, mapped);
	this->_stock.insert(elem);

}

void				Request::parseBuf()
{
	std::string& buf = this->_request;

	this->_response_status_code = 0;
	this->_err_referer = false;
	this->_url_dir = false;

	std::stringstream	b(buf);
	std::string 		line;

	std::getline(b, line);
	this->setMethod(line);
	this->setUrl(line);
	this->setProtocolVersion(line);

	while (std::getline(b, line))
		this->buildMap(line);

	this->setHost();
	this->setUserAgent();
	this->setAccept();
	this->setAcceptLanguage();
	this->setAcceptEncoding();
	this->setConnection();
	this->setReferer();

	this->set_contentLength();
	this->set_contentType();

	this->setBody(buf);
	this->set_queryString();

	this->treatUrl();
	this->_url = trim_double_slash(this->_url);

	// std::cout << YELLOW << "\nURL Processed-> " << this->_url << NC << '\n';

	this->defineProtocolVersion();
	this->defineStatusCode();
	this->defineUrl();

	// std::cout << CYAN << "URL Send-> " << this->_response_url << " " << this->_response_status_code <<  NC << '\n';

	this->_connexion_end = true;
}

void				Request::defineProtocolVersion()
{
	this->_response_protocol_version = this->_protocol_version;
}

std::string			test_url_toDelete(std::string url)
{
	struct stat buffer;
	std::string tmp_url = url;
	if (stat(tmp_url.c_str(), &buffer) == -1)
	{
		tmp_url.clear();
		tmp_url.assign(url.begin(), url.begin() + url.find_last_of('/'));
		if (stat(tmp_url.c_str(), &buffer) == -1)
			return ("");
	}
	return (tmp_url);
}

void				Request::defineStatusCode()
{
	if (this->_response_status_code != 0)
		return ;
	if (this->_method == "DELETE")
	{
		std::string url_toDelete = test_url_toDelete(this->_url);
		if (url_toDelete.empty() == true)
		{
			this->_response_status_code = 404;
			return ;
		}
		if (remove (url_toDelete.c_str()) == 0)
		{
			this->_response_status_code = 200;
			return ;
		}
		else
		{
			std::cerr << RED << "ERROR DELETE: " << NC << std::strerror(errno) << '\n';
			this->_response_status_code = 500;
			return ;
		}
	}

	std::ifstream ms;

	ms.close();
	ms.open(this->_url);
	if (ms.is_open() == true)
		this->_response_status_code = 200;
	else
		this->_response_status_code = 404;
	ms.close();

	return ;
}

void		Request::defineUrl()
{
	if (this->_response_status_code < 400)
		this->_response_url = this->_url;
	else
	{
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
	return ;
}

std::string			Request::treat_referer(std::string ref)
{
	std::string ret = ref;
	std::string localhost("localhost");

	ret.erase(ret.begin(), ret.begin() + 7);

	std::string host = this->_block->get_host();
	if (ret.size() > host.size())
	{
		std::string tmp(ret.begin(), ret.begin() + host.size());
		std::string tmp2(ret.begin(), ret.begin() + localhost.size());
		if (tmp.compare(host) == 0)
			ret.erase(ret.begin() , ret.begin() + tmp.size() + 1);
		else if (tmp2.compare(localhost) == 0)
			ret.erase(ret.begin() , ret.begin() + tmp2.size() + 1);
	}
	std::string port = std::to_string(this->_block->get_port());
	if (ret.size() > port.size())
	{
		std::string tmp(ret.begin(), ret.begin() + port.size());
		if (tmp.compare(port) == 0)
			ret.erase(ret.begin() , ret.begin() + tmp.size());
	}
	return (ret);
}

/*	All getters (one for each attribute corresponding to a field of the HTTP _request):
*/
const std::string&	Request::getMethod() const{ return (this->_method); }
const std::string&	Request::getUrl() const{ return (this->_url); }
const std::string&	Request::getBaseUrl() const { return (this->_base_url); }
const std::string&	Request::getProtocolVersion() const{ return (this->_protocol_version); }
const std::string&	Request::getHost() const{ return(this->_host); }
const std::string&	Request::getUserAgent() const{ return(this->_user_agent); }
const std::string&	Request::getAccept() const{ return(this->_accept); }
const std::string&	Request::getAcceptLanguage() const{ return(this->_accept_language); }
const std::string&	Request::getAcceptEncoding() const{ return (this->_accept_encoding); }
const std::string&	Request::getConnection() const{ return(this->_connection); }
const std::string&	Request::getReferer() const{ return(this->_referer); }
const std::string&	Request::getBody() const{ return(this->_body); }
const std::vector<unsigned char> Request::getVecBody() const { return (this->_vec_body); }

serv_block*	Request::getBlock() { return(this->_block); }
const bool&	Request::is_request_ready() const { return (this->_request_ready); }
const bool& Request::get_url_dir() const { return (this->_url_dir); }
const std::string&	Request::returnProtocolVersion() const{ return (this->_response_protocol_version); }
const std::string&	Request::returnUrl() const{ return (this->_response_url); }
int					Request::returnStatusCode() const{ return (this->_response_status_code); }

const std::string&			Request::get_queryString() const{ return(this->_queryString); }
const std::string&			Request::get_contentLength() const{ return(this->_content_length); }
const std::string&			Request::get_contentType() const{return(this->_content_type); }

const std::string&			Request::getRequest() const{return(this->_request); }

std::ostream&		operator<<(std::ostream& os, const Request& r)
{
	os << "METHOD:" << NC << " [" << r.getMethod() << "]" << CYAN << std::endl;
	os << "PROCESSED URL:" << NC << " [" << r.getUrl() << "]" << CYAN << std::endl;
	os << "REQUESTED URL:" << NC << " [" << r.getBaseUrl() << "]" << CYAN << std::endl;
	os << "PROTOCOLE VERSION:" << NC << " [" << r.getProtocolVersion() << "]" << CYAN << std::endl;
	os << "HOST:" << NC << " [" << r.getHost() << "]" << CYAN << std::endl;
	os << "USER AGENT:" << NC << " [" << r.getUserAgent() << "]" << CYAN << std::endl;
	os << "ACCEPT:" << NC << " [" << r.getAccept() << "]" << CYAN << std::endl;
	os << "ACCEPT LANGUAGE:" << NC << " [" << r.getAcceptLanguage() << "]" << CYAN << std::endl;
	os << "ACCEPT ENCODING:" << NC << " [" << r.getAcceptEncoding() << "]" << CYAN << std::endl;
	os << "CONNECTION:" << NC << " [" << r.getConnection() << "]" << CYAN << std::endl;
	os << "REFERER:" << NC << " [" << r.getReferer() << "]" << CYAN << std::endl;
	// os << "BODY:" << NC << " [" << r.getBody() << "]" << CYAN << std::endl;
	os << "QUERY STRING:" << NC << " [" << r.get_queryString() << "]" << CYAN << std::endl;
	os << "CONTENT LENGTH:" << NC << " [" << r.get_contentLength() << "]" << CYAN << std::endl;
	os << "CONTENT TYPE:" << NC << " [" << r.get_contentType() << "]" << CYAN << std::endl;
	return (os);
}
