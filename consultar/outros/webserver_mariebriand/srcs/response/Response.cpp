/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 15:24:59 by mabriand          #+#    #+#             */
/*   Updated: 2022/02/18 23:54:05 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Response.hpp"
#include "../request/Request.hpp"

Response::Response(const Request &req, serv_block *block, bool cgi, std::vector<unsigned char> cgiOutput) : _req(req), _block(block)
{
	this->_target_dir = this->_req.get_url_dir();

		// Independant from the parameters
	this->setMimeMap();
	this->setMessagesMap();

		// Dependant from the parameters
	this->setProtocolVersion(this->_req.returnProtocolVersion());
	this->setStatus(this->_req.returnStatusCode());
	this->setStatusMessage(this->_req.returnStatusCode());
	if (this->_req.returnStatusCode() == 300 )
		this->setLocation(this->_req.returnUrl());
	this->setDate();
	this->setServer();
	this->setConnection();
	this->setKeepAlive();

	if (cgi == false && (this->_req.getMethod() != "POST" || this->_req.returnStatusCode() >= 400))
	{
		this->setBody(this->_req.returnUrl());
		this->setContentType(this->_req.returnUrl());
	}
	else
		this->set_cgiOutput(cgiOutput, cgi);
	this->setContentLenght();

	this->buildResponse();

	std::cout << '\n' << BLUE << "CODE RETURNED: " << this->_stock["Status"] << ":" << this->_stock["Status-Message"] << NC << '\n';
	return ;

}
Response::~Response(){}

void				Response::setProtocolVersion(const std::string& protocol_version)
{
	if (this->_req.returnStatusCode() == 400)
		this->_protocol_version = "HTTP/1.1 ";
	else
		this->_protocol_version = protocol_version + " ";

	std::pair<std::string, std::string> elem("Protocol-Version", this->_protocol_version);
	this->_stock.insert(elem);

	return ;
}
void				Response::setStatus(int status)
{
	std::stringstream	out;

	out << status;
	this->_status = out.str() + " ";

	std::pair<std::string, std::string> elem("Status", this->_status);
	this->_stock.insert(elem);

	return ;
}
void				Response::setStatusMessage(int status)
{
	std::map<int, std::string>::iterator	itf = (this->_messages).find(status);
	this->_status_message = itf->second;
	std::pair<std::string, std::string> elem("Status-Message", this->_status_message);
	this->_stock.insert(elem);

	return ;
}
void				Response::setLocation(std::string url)
{
	std::string location(url.begin(), url.begin() + url.find_last_of('/'));

	std::pair<std::string, std::string> elem("Location", location);
	this->_stock.insert(elem);

	return ;
}
void				Response::setDate()
{
	std::time_t result = std::time(nullptr);
	std::string date(std::asctime(std::localtime(&result)));
	date.pop_back();

	this->_date = date;
	std::pair<std::string, std::string> elem("Date", this->_date);
	this->_stock.insert(elem);

	return ;
}
void				Response::setServer()
{
	this->_server = this->_block->get_server_name();

	std::pair<std::string, std::string> elem("Server", this->_server);
	this->_stock.insert(elem);

	return ;
}
void				Response::setConnection()
{
	this->_connection = "keep-alive";

	std::pair<std::string, std::string> elem("Connection", this->_connection);
	this->_stock.insert(elem);

	return ;
}
void				Response::setKeepAlive()
{
	this->_keep_alive = "timeout=3";

	std::pair<std::string, std::string> elem("Keep-Alive", this->_keep_alive);
	this->_stock.insert(elem);

	return ;
}
void				Response::setContentType(const std::string& url)
{
	size_t pos = url.find_last_of(".", url.size());
	if (pos == std::string::npos)
		return ;

	std::string key(url.substr(pos, url.size() - pos));
	std::map<std::string, std::string>::iterator	itf = (this->_mime).find(key);
	this->_content_type = itf->second;
	std::pair<std::string, std::string> elem("Content-Type", this->_content_type);
	this->_stock.insert(elem);
	this->_selected_mime = key;
	return ;
}

void				Response::setContentLenght()
{
	std::stringstream content_length;
	content_length << this->_body.size();

	this->_content_length = content_length.str();
	std::pair<std::string, std::string> elem("Content-Length", this->_content_length);
	this->_stock.insert(elem);

	return ;
}

bool IsPathExist(const std::string &s)
{
	struct stat buffer;
	return (stat (s.c_str(), &buffer) == 0);
}

void				Response::setBody(const std::string& url)
{
	std::string tmp_url = this->_req.getUrl();
	std::string url_dir(tmp_url.begin(), tmp_url.begin() + tmp_url.find_last_of('/'));
	bool path_exist = IsPathExist(url_dir);

	if (url_dir == ".")
		path_exist = false;

	if (this->_block->get_autoindex() && this->_req.returnStatusCode() == 404 && this->_target_dir && path_exist)
	{
		std::string test = build_autoindex_body(url_dir.c_str());
		for (size_t i = 0; i < test.size(); i++)
			this->_body.push_back(test[i]);
		this->_stock["Status"] = "200 ";
	}
	else
	{
		std::ifstream ms(url.c_str(), std::ios::in | std::ios::binary);

		if (!ms.eof() && !ms.fail())
		{
			ms.seekg(0, std::ios_base::end);
			std::streampos fileSize = ms.tellg();
			this->_body.resize(fileSize);

			ms.seekg(0, std::ios_base::beg);
			ms.read((char*)&this->_body[0], fileSize);
		}

		ms.close();
	}
	std::vector<unsigned char>::iterator	it = this->_body.begin();
	std::vector<unsigned char>::iterator	ite = this->_body.end();
	std::string	tmp(it, ite);
	this->_bodyStr = tmp;
	return ;
}
void				Response::set_newContentType(std::vector<unsigned char> header)
{
	std::string str(header.begin(), header.end());

	size_t pos = str.find_last_of(":", str.size());
	if (pos == std::string::npos)
		return ;

	std::string type(str.substr(pos + 2, str.size() - (pos + 2)));

	this->_content_type = type;

	std::pair<std::string, std::string> elem("Content-Type", this->_content_type);
	this->_stock.insert(elem);
	this->_selected_mime = type;

	return ;
}
void				Response::set_cgiOutput(std::vector<unsigned char> cgi_output, bool cgi)
{
	std::vector<unsigned char>	head;
	std::vector<unsigned char>	type;
	std::vector<unsigned char>	body;

	if (cgi == true)
	{
		std::string str_output(cgi_output.begin(), cgi_output.end());

		size_t pos;
		while ((pos = str_output.find("\r")) != std::string::npos)
			str_output.replace(pos, 1, "\n");

		pos = str_output.find_first_of("\n", 0);
		std::string str_head(str_output.substr(0, pos));
		while (str_output[pos] == '\n')
			++pos;
		str_output.erase(0, pos);

		pos = str_output.find_first_of("\n", 0);
		std::string str_type(str_output.substr(0, pos));
		while (str_output[pos] == '\n')
			++pos;
		str_output.erase(0, pos);

		size_t i = 0;
		while (i < str_head.size())
		{
			head.push_back(str_head[i]);
			++i;
		}
		i = 0;
		while (i < str_type.size())
		{
			type.push_back(str_type[i]);
			++i;
		}
		i = 0;
		while (i < str_output.size())
		{
			body.push_back(str_output[i]);
			++i;
		}
		this->_cgi_body = body;
		this->_body = body;
		this->_cgi_head = head;
		this->set_newContentType(type);


		i = 0;
	}
	else
	{
		this->_stock["Status"] = "204 ";
		this->_stock["Status-Message"] = this->_messages.find(204)->second;
		std::pair<std::string, std::string> elem("Content-Type", this->_req.get_contentType());
		this->_stock.insert(elem);
	}
	return ;
}

const std::string&	Response::get_ProtocolVersion() const{ return (this->_protocol_version); }
const std::string&	Response::get_Status() const{ return (this->_status); }
const std::string&	Response::get_StatusMessage() const{ return (this->_status_message); }
const std::string&	Response::get_Date() const{ return (this->_date); }
const std::string&	Response::get_Server() const{ return (this->_server); }
const std::string&	Response::get_ContentType() const{ return (this->_content_type); }
const std::string&	Response::get_ContentLenght() const{ return (this->_content_length); }
const std::string&	Response::get_bodyStr() const{ return(this->_bodyStr); }
const std::vector<unsigned char>&	Response::getBody() const{ return(this->_body); }
const std::string&  Response::get_Mime() const{ return (this->_selected_mime); }
//
const std::vector<unsigned char>& Response::getVecResponse() const {return (this->_response); }


void				Response::fill_resp(const std::string& line)
{
	this->_response.insert(this->_response.end(), line.begin(), line.end());
}

void				Response::buildPartResp(const std::string& key)
{
	std::map<std::string, std::string>::iterator	itf = (this->_stock).find(key);

	if (key.compare("Protocol-Version") == 0 || key.compare("Status") == 0)
		fill_resp(itf->second);
	else if (key.compare("Location") == 0)
		fill_resp(itf->first + ": " + itf->second + "\r\n");
	else if (key.compare("Status-Message") == 0)
		fill_resp(itf->second + "\r\n");
	else
		fill_resp(itf->first + ": " + itf->second + "\r\n");
	return ;
}
void				Response::buildResponse()
{
	this->buildPartResp("Protocol-Version");
	this->buildPartResp("Status");
	this->buildPartResp("Status-Message");

	if (this->_status.compare("300 ") == 0)
		this->buildPartResp("Location");
	this->buildPartResp("Date");
	this->buildPartResp("Server");
	this->buildPartResp("Connection");
	this->buildPartResp("Keep-Alive");
	this->buildPartResp("Content-Type");
	this->buildPartResp("Content-Length");

	if (this->_body.empty() == false)
	{
		this->fill_resp("\r\n");
		this->_response.insert(this->_response.end(), this->_body.begin(), this->_body.end());
	}
	return ;
}
std::ostream&		operator<<(std::ostream& os, const Response& r)
{
	os << "[" << r.get_ProtocolVersion() << "]" << std::endl;
	os << "[" << r.get_Status() << "]" << std::endl;
	os << "[" << r.get_StatusMessage() << "]" << std::endl;
	os << "[" << r.get_Date() << "]" << std::endl;
	os << "[" << r.get_Server() << "]" << std::endl;
	os << "[" << r.get_ContentType() << "]" << std::endl;
	os << "[" << r.get_ContentLenght() << "]" << std::endl;
	os << "[" << r.get_bodyStr() << "]" << std::endl;

	return (os);
}
