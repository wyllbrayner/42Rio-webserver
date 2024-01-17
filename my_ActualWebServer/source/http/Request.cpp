#include "Request.hpp"

/*******************************************************/
/*						Constructors.		           */
/*******************************************************/

Request::Request(int newClient, ConfigFile _configFile)
{
	this->_fromClient = newClient;
	this->_delimeter = "\r\n\r\n";
	this->_serverConf = _configFile;
	this->reset();
}

Request::~Request(void) {}

/*******************************************************/
/*				Getters of HTTP request.               */
/*******************************************************/

void					Request::setStart_time(time_t start_time)
{
	this->_start_time = start_time;
}

const time_t &			Request::getStartTime(void) const
{
	return (this->_start_time);
}

const std::string							&	Request::getMethod(void) const
{
	return (this->_method);
}

const std::string							&	Request::getLocation(void) const
{
	return (this->_location);
}

const std::string							&	Request::getRequestedInf(void) const
{
	return (this->_requestedInf);
}

const std::string							&	Request::getContentType(void) const
{
	return (this->_contentType);
}

const std::string							&	Request::getBoundary(void) const
{
	return (this->_boundary);
}

const ConfigFile							&	Request::getServerConf(void) const
{
	return (this->_serverConf);
}

const std::map<std::string, std::string>	&	Request::getMapQueryString(void) const
{
	return (this->_mapQueryString);
}

const std::string							&	Request::getQueryStringS(void) const
{
	return (this->_queryStringS);
}

const std::string							&	Request::returnBody(void) const
{
	return (this->_body);
}

const std::string							&	Request::getUserAgent(void) const
{
	return (this->_userAgent);
}

const std::string							&	Request::getHost(void) const
{
	return (this->_host);
}

const std::string							&	Request::returnHeader(void) const
{
	return (this->_header);
}

const std::string								Request::returnPort(void) const
{
	std::stringstream	ss;

	ss << this->getServerConf().getPort()[0];
	std::string str = ss.str();
	return (str);
}

int												Request::totalLength(void)
{
    return (this->_contentLength);
}

std::string										Request::totalLengthS(void) const
{
	std::stringstream	ss;

	ss << _contentLength;
	std::string str = ss.str();
	return (str);
}

bool											Request::isReady(void) const
{
    return (this->_ready);
}

const size_t								&	Request::getContentLength(void) const
{
    return (this->_contentLength);
}

bool											Request::receiveFromClient(int client)
{
	char	buffer[BUFFER_SIZE];
	int		bytes;

	bytes = recv(client, buffer, BUFFER_SIZE - 1, 0);
    if (this->checkBytesReceived(bytes) != 1)
	{
		std::cout << "fim    | receiveFromClient: " << client << std::endl;
		return (false);
	}
	buffer[bytes] = '\0';
/*
	std::cout << "Round: Bodysize: " << this->_body.size() \
				<< " | I read now: " << bytes << " bytes." << std::endl;
	printYellow("content start");
	std::cout << buffer << std::endl;
	printYellow("content end");
*/
	if (!this->getHeader(buffer))
		return (false);
	this->getBody(buffer, bytes);
	return (true);
}

int												Request::checkBytesReceived(ssize_t bytes_received)
{
	if (bytes_received == -1)
		return (-1);
	else if (bytes_received == 0)
	{
		std::cout << "Client disconnected" << std::endl;
		return (0);
	}
	return (1);
}

bool											Request::getHeader(std::string const& buffer)
{
	size_t	pos;

    if (!this->_header.empty())
        return (true);
	else
	{
		pos = buffer.find(this->_delimeter);
		if (pos == std::string::npos)
		{
			printYellow("I didn't find the delimeter in getHeader");
			return (false);
		}
		this->_header.append(buffer.begin(), buffer.begin() + pos);
		if (!this->parseRequest())
			return (false);
		return (true);
	}
}

void											Request::getBody(std::string const& buffer, int bytes)
{
    this->appendTheBody(buffer, bytes);
    if (this->_body.size() == this->_contentLength)
    {
        this->_ready = true;
		this->_httpMessage = this->_header + this->_body;
		printYellow("Reached the size");
/*
		printYellow("body start");
		std::cout << this->_body << std::endl;
		printYellow("body end");
*/
    }
}

void											Request::appendTheBody(std::string buffer, const int bytes)
{
	size_t i;

    if (this->_body.empty())
    {
		if (this->getBoundary().empty())
		{
			i = buffer.find(this->_delimeter) + this->_delimeter.size();
			this->_body.append(buffer.begin() + i, buffer.end());
		}
		else
		{
			this->fixebuffer(buffer);
			i = buffer.find(this->getBoundary());
			if (i != std::string::npos)
				this->_body.append(buffer.begin() + i, buffer.end());
		}
    }
	else
		this->_body.append(buffer.begin(), buffer.begin() + bytes);
}

void											Request::fixebuffer(std::string & buffer)
{
	size_t	start;
	size_t	end;

	start = 0;
	end = 0;
	this->findStartEnd(start, end, " boundary=", buffer);
	if (start != std::string::npos && end != std::string::npos)
		buffer = buffer.substr(start + this->getBoundary().size());
}

/*******************************************************/
/*				Parse of HTTP request.					*/
/*******************************************************/

bool											Request::parseRequest(void)
{
	if (this->_header.empty() || !this->_method.empty())
		return (true);
	this->findContentLength();
	this->findContentType();
	this->findHost();
	this->findUserAgent();
	this->findBoundary();
	if (!this->splitRequest(this->_header.substr(0, this->_header.find(" HTTP/")), \
						this->_serverConf.getRoot()))
		return (false);
	return (true);
}

void											Request::findContentLength(void)
{
	size_t	start;
	size_t	end;

	start = this->_header.find("Content-Length: ");
	if (start == std::string::npos)
	{
		this->_ready = true;
		printYellow("The request is ready without body");
	}
	else
	{
		start = 0;
		end = 0;
		this->findStartEnd(start, end, "Content-Length: ", this->_header);
		if (start != std::string::npos && end != std::string::npos)
			this->_contentLength = \
					Utils::atoi(this->_header.substr(start, (end - start + 1)));
	}
}

void											Request::findContentType(void)
{
	size_t	start;
	size_t	end;

	start = this->_header.find("Content-Type:");
	if (start != std::string::npos)
	{
		end = 0;
		start += 13;
		while (this->_header[start] == ' ')
			start++;
		while (this->_header[start + end] != '\r')
			end++;
		this->_contentType = this->_header.substr(start, end);
	}
}

void											Request::findHost(void)
{
	size_t	start;
	size_t	end;

	start = 0;
	end = 0;
	this->findStartEnd(start, end, "Host: ", this->_header);
	if (start != std::string::npos && end != std::string::npos)
		this->_host = this->_header.substr(start, (end - start + 1));
}

void											Request::findUserAgent(void)
{
	size_t	start;
	size_t	end;

	start = this->_header.find("User-Agent:");
	if (start != std::string::npos)
	{
		end = 0;
		start += 12;
		while (this->_header[start] == ' ')
			start++;
		while (this->_header[start + end] != '\r')
			end++;
		this->_userAgent = this->_header.substr(start, end);
	}
}

void											Request::findBoundary(void)
{
	size_t	start;
	size_t	end;

	start = 0;
	end = 0;
	this->findStartEnd(start, end, " boundary=", this->_header);
	if (start != std::string::npos && end != std::string::npos)
		this->_boundary.append("--").append(this->_header.substr(start, (end - start + 1)));
}

bool											Request::splitRequest(std::string urlRequest, std::string root)
{
	std::vector<std::string>::iterator	i;
	std::vector<std::string>			splitHeadRequest;
	std::string							tmp;
	size_t								j;

/*
	if (urlRequest.find("/favicon.ico") != std::string::npos || \
			urlRequest.find("OPTIONS") != std::string::npos)
		this->fixeUrlRequest(urlRequest);
*/
	if (urlRequest.find("/favicon.ico") != std::string::npos)
		return (false);
	if (urlRequest.find("OPTIONS") != std::string::npos)
		this->fixeUrlRequest(urlRequest);
	if (urlRequest.compare(0, 12, "requestError") != 0)
	{
		splitHeadRequest = Utils::split(urlRequest, " \t\n");
		i = splitHeadRequest.begin();
		if (this->_method.empty())
			this->_method = *(i++);
		j = (*i).rfind("/");
		this->_location = root.append(Utils::setPlace((*i).substr(0, (j + 1))));
		this->_requestedInf = (*i).substr((j + 1), ((*i).size() - (j + 1)));
		j = this->_requestedInf.find("?");
		if (j != std::string::npos)
		{
			this->_queryStringS = this->_requestedInf.substr((j + 1));
			this->_mapQueryString = this->parseQueryString(this->_queryStringS);
			this->_requestedInf = this->_requestedInf.substr(0, j);
		}
	}
	if (this->_requestedInf.empty())
		this->_requestedInf = this->_serverConf.getIndex()[0];
	else
	{
		j = 0;
		while (j < this->_requestedInf.size())
		{
			if (!std::isprint(this->_requestedInf[j]))
			{
				tmp = this->_requestedInf.substr(0, j);
				this->_requestedInf.clear();
				this->_requestedInf = tmp;
				break ;
			}
			j++;
		}
	}
/*
	std::cout << "method: " << this->_method << " _location: " << this->_location << " requestedInf: " << this->_requestedInf;
	std::map<std::string, std::string>::iterator a = this->_mapQueryString.begin();
	std::map<std::string, std::string>::iterator z = this->_mapQueryString.end();
	if (a != z)
		std::cout << " queryString:";
	while (a != z)
	{
		std::cout << " key: " << a->first << " value: " << a->second;
		a++;
	}
	std::cout << std::endl;
*/
	return (true);
}

void											Request::fixeUrlRequest(std::string & urlRequest)
{
	if (urlRequest.find("OPTIONS") != std::string::npos)
	{
		urlRequest = this->fixeUrlRequestAux(urlRequest, \
						"OPTIONS", "Access-Control-Request-Method: ");
	}
	if (urlRequest.find("/favicon.ico") != std::string::npos)
	{
		urlRequest = this->fixeUrlRequestAux(urlRequest, \
						"/favicon.ico", "Referer: http://");
	}
}

std::string										Request::fixeUrlRequestAux(\
	std::string & url, const std::string oldValue, const std::string toFind)
{
	size_t		pos;
	size_t		start;
	size_t		end;
	std::string	str;

	pos = url.find(oldValue);
	start = 0;
	end = 0;
	if (pos != std::string::npos)
	{
		this->findStartEnd(start, end, toFind, this->_header);
		if (start != std::string::npos && end != std::string::npos)
		{
			if (oldValue.compare(0, oldValue.size(), "OPTIONS") == 0)
			{
				if (this->_header.substr(start, (end - start + 1))\
					.compare(0, 6, "DELETE") == 0)
					this->_method = "DELETE";
				else
					this->_method = this->_header.substr(start, (end - start + 1));
				url = url.substr(oldValue.size() + 1);
			}
			else
			{
				str = this->_header.substr(start, (end - start + 1));
				url.erase(pos, oldValue.size())\
					.insert(pos, str.substr(str.find("/")));
			}
		}
		else
			url = "requestError";
	}
	else
		url = "requestError";
	return (url);
}

void											Request::findStartEnd(\
	size_t & start, size_t & end, const std::string toFind, const std::string & place)
{
	start = place.find(toFind);
	if (start != std::string::npos)
	{
		start += toFind.size();
		if (start < place.size())
		{
			end = start;
			while ((end < place.size()) && std::isspace(place[end]))
				end++;
			while ((end < place.size()) && !std::isspace(place[end]))
				end++;
			end--;
		}
		else
			start = std::string::npos;
	}
	else
		start = std::string::npos;
}

std::map<std::string, std::string>				Request::parseQueryString(\
	std::string queryString)
{
	std::map<std::string, std::string>	mapQueryString;
	std::vector<std::string>			splitQueryString;
	size_t								i;
	size_t								j;
	std::string							key;
	std::string							value;

	splitQueryString = Utils::split(queryString, "&");
	i = 0;
	while (i < splitQueryString.size())
	{
		j = splitQueryString[i].find("=");
		key = splitQueryString[i].substr(0, j);
		value = splitQueryString[i].substr((j + 1));
		mapQueryString[this->urlDecoder(key)] = this->urlDecoder(value);
		i++;
	}
	return (mapQueryString);
}

std::string										Request::urlDecoder(\
	const std::string & url)
{
	size_t		i;
	int			hexVal;
	std::string	ret;

	i = 0;
	while (i < url.size())
	{
		if (url.compare(i, 1, "+") == 0)
			ret += ' ';
		else if ((url.compare(i, 1, "%") == 0) && ((i + 2) < url.size()))
		{
			std::istringstream	hexStream(url.substr((i + 1), 2));
			hexStream >> std::hex >> hexVal;
			ret += static_cast<char>(hexVal);
			i += 2;			
		}
		else
			ret += url[i];
		i++;
	}
	return (ret);
}

/********************************************************/
/*				Reset methods.							*/
/********************************************************/

void											Request::reset(void)
{
	this->_ready = false;
	this->_contentLength = 0;
	this->_header.clear();
	this->_body.clear();
	this->_httpMessage.clear();
	this->_method.clear();
	this->_location.clear();
	this->_requestedInf.clear();
	this->_contentType.clear();
	this->_boundary.clear();
	this->_queryStringS.clear();
	this->_mapQueryString.clear();
}

void											Request::printYellow(\
	const std::string & str) const
{
	std::cout << "\033[1;33m" << str << "\033[0m" << std::endl;
}
