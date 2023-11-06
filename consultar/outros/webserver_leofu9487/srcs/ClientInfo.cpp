#include "../includes/webserv.hpp"

ClientInfo::ClientInfo(ServerInfo const &server):_server(server), _request(""), _ready(false), _response("")
{
	
}

void	ClientInfo::add_request(std::string const &str)
{
	_request += str;
}

void	ClientInfo::set_ready()
{
	_ready = true;
}

void	ClientInfo::set_response(std::string const &str)
{
	_response = str;
}

std::string const &ClientInfo::get_request() const
{
	return _request;
}

ServerInfo const &ClientInfo::get_server() const
{
	return _server;
}

bool	ClientInfo::get_ready() const
{
	return _ready;
}

bool	ClientInfo::request_is_complete() const
{
	std::stringstream	ss(_request);

	std::string method;
	std::string path;
	std::string HTTP_version;
	std::string boundary;

	if (!(ss >> method))
		return false;
	if (!(ss >> path))
		return false;
	if (!(ss >> HTTP_version))
		return false;

	std::string tmp;
	if (method == "POST")
	{	
		while (ss >> tmp)
		{

			if (tmp.substr(0, 9) == "boundary=")
			{
				boundary = tmp.substr(9);
			}
		}
	}

	if (boundary != "")
	{
		// std::cerr << "\n" << tmp <<"\n";
		return tmp.substr(0, 4 + boundary.size()) == "--" + boundary + "--";
	}

	return true;
}

std::string const &ClientInfo::get_response() const
{
	return _response;
}
