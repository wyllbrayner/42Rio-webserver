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
//	this->_ready = false;
//	this->_contentLength = 0;
//	this->_serverConf.printConfigFile();
/*
	std::cout << "construtor da Request port: " << this->_serverConf.getPort() << " path: " << this->_serverConf.getRoot() << " getIndex: " << this->_serverConf.getIndex()[0] << std::endl;
	std::cout << "tamanho do location: " << this->_serverConf.getLocation().size() << std::endl;
	size_t	i = 0;
	while (i < this->_serverConf.getLocation().size())
	{
		std::cout << "Location path: " << this->_serverConf.getLocation()[i].getPath() << std::endl;
		i++;
	}
*/
}

Request::~Request(void) {}

/*******************************************************/
/*				Getters of HTTP request.               */
/*******************************************************/

const std::string &		Request::getMethod(void) const
{
	return (this->_method);
}

const std::string &		Request::getLocation(void) const
{
	return (this->_location);
}

const std::string &		Request::getRequestedInf(void) const
{
	return (this->_requestedInf);
}

const std::string &		Request::getContentType(void) const
{
	return (this->_contentType);
}

const ConfigFile  &		Request::getServerConf(void) const
{
	return (this->_serverConf);
}

const std::map<std::string, std::string> &		Request::getMapQueryString(void) const
{
	return (this->_mapQueryString);
}

const std::string &		Request::getQueryString(void) const
{
	return (this->_queryString);
}

bool    	Request::isReady(void) const
{
    return (this->_ready);
}

const size_t		&	Request::getContentLength(void) const
{
    return (this->_contentLength);
}

bool		Request::receiveFromClient(int client)
{
	std::cout << "inicio | receiveFromClient: " << client << std::endl;

	char	buffer[BUFFER_SIZE];
	int		bytes;

	bytes = recv(client, buffer, BUFFER_SIZE - 1, 0);
    if (this->checkBytesReceived(bytes) != 1)
		return (false);
	buffer[bytes] = '\0';
/*
	std::cout << "Round: Bodysize: " << this->_body.size() \
				<< " | I read now: " << bytes << std::endl;
*/
	if (!this->getHeader(buffer))
		return (false);
	this->getBody(buffer, bytes);
	if (this->isReady() && this->getMethod().compare(0, 4, "POST") == 0 && \
		getContentType().compare(0, 33 , "application/x-www-form-urlencoded") \
		== 0 && this->getContentLength() != 0 && !this->_body.empty() && \
		this->getMapQueryString().empty())
	{
		this->_queryString = this->_body;
		this->_mapQueryString = this->parseQueryString(this->_queryString);
/*
		std::cout << "body isReady!!!!" << std::endl;
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
	}
//	else
//		std::cout << "body Falhou!!!!" << std::endl;
//	std::cout << "queryString: " << this->getQueryString() << std::endl;
	std::cout << "fim    | receiveFromClient: " << client << std::endl;
	return (true);
}



int	Request::checkBytesReceived(ssize_t bytes_received)
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

bool		Request::getHeader(std::string const& buffer)
{
	std::cout << "inicio | getHeader buffer: " << std::endl;
//	std::cout << "inicio | getHeader buffer: " << buffer << std::endl;
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
//		std::cout << "_header: " << this->_header << std::endl;
//		this->parseRequest();
		if (!this->parseRequest())
			return (false);
//		printYellow("header: " + this->_header);
		std::cout << "fim    | getHeader " << std::endl;
		return (true);
	}
}

void		Request::getBody(std::string const& buffer, int bytes)
{
	std::cout << "Início | getBody" << std::endl;
    this->appendTheBody(buffer, bytes);
    if (this->_body.size() == this->_contentLength)
    {
        this->_ready = true;
		this->_httpMessage = this->_header + this->_body;
		printYellow("Reached the size");
//		printYellow("BODY: " + this->_body);
    }
	std::cout << "fim    | getBody " << this->_body << std::endl;
}

void        Request::appendTheBody(std::string const& buffer, int bytes)
{
	size_t i;

    if (this->_body.empty())
    {
		i = buffer.find(this->_delimeter) + this->_delimeter.size();
        this->_body.append(buffer.begin() + i, buffer.end());
    }
	else
		this->_body.append(buffer.begin(), buffer.begin() + bytes);
}

/*******************************************************/
/*				Parse of HTTP request.					*/
/*******************************************************/

bool	Request::parseRequest()
{
	std::cout << "Início | parseRequest: " << std::endl;
//	std::cout << "Início | parseRequest: " << this->_header << std::endl;
	if (this->_header.empty() || !this->_method.empty())
		return (true);
	this->findContentLength();
	this->findContentType();
/*
	if (!this->splitRequest(this->_header.substr(0, this->_header.find(" HTTP/")), \
						this->_serverConf.getRoot()))
		return (false);
*/
	this->splitRequest(this->_header.substr(0, this->_header.find(" HTTP/")), \
						this->_serverConf.getRoot());
//	std::cout << "method: " << this->_method << " location: " << this->_location << " request inf: " << this->_requestedInf << std::endl;
	std::cout << "fim    | parseRequest: " << this->_header << std::endl;
	return (true);
}

// Check back error handling
void	Request::findContentLength(void)
{
	std::cout << "inicio | findContentLength: " << std::endl;
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
//		std::cout << "contentLength: " << this->_contentLength << std::endl;
	}
 	std::cout << "fim    | findContentLength: " << this->_contentLength << std::endl;
}

void	Request::findContentType(void)
{
	std::cout << "inicio | findContentType: " << std::endl;
	size_t	start;
	size_t	end;

	start = 0;
	end = 0;
	this->findStartEnd(start, end, "Content-Type: ", this->_header);
	if (start != std::string::npos && end != std::string::npos)
		this->_contentType = this->_header.substr(start, (end - start + 1));
 	std::cout << "fim    | findContentType: " << this->_contentType << std::endl;
}

void	Request::splitRequest(std::string urlRequest, std::string root)
{
	std::cout << "início | splitRequest urlRequest: " << urlRequest << " e root: " << root << std::endl;
	std::vector<std::string>::iterator	i;
	std::vector<std::string>			splitHeadRequest;
	std::string							tmp;
	size_t								j;

	if (urlRequest.find("/favicon.ico") != std::string::npos || \
			urlRequest.find("OPTIONS") != std::string::npos)
		this->fixeUrlRequest(urlRequest);
/*
	if (urlRequest.find("/favicon.ico") != std::string::npos)
		return (false);
	if (urlRequest.find("OPTIONS") != std::string::npos)
		this->fixeUrlRequest(urlRequest);
*/
	if (urlRequest.compare(0, 12, "requestError") != 0)
	{
		splitHeadRequest = Utils::split(urlRequest, " \t\n");
//		std::cout << "\t\turlRequest: " << urlRequest << std::endl;
//		std::cout << "\t\tpassou da split sem segfalt com splitHeadRequest.size(): " << splitHeadRequest.size() << std::endl;
		i = splitHeadRequest.begin();
//		std::cout << "\t\tthis->_method: " << this->_method << std::endl;
		if (this->_method.empty())
			this->_method = *(i++);
//		else
//			std::cout << "método já preenchido com: " << this->_method << std::endl;
//		std::cout << "\t\tthis->_method: " << this->_method << std::endl;
//		std::cout << "\t\tpassou do _method sem segfalt com splitHeadRequest.size(): " << splitHeadRequest.size() << " this->_method: " << this->_method << std::endl;
		j = (*i).rfind("/");
//		std::cout << "\t\tpassou do rfind(\"/\") sem segfalt com splitHeadRequest.size(): " << splitHeadRequest.size() << " " << this->_method << std::endl;
		this->_location = root.append(Utils::setPlace((*i).substr(0, (j + 1))));
		this->_requestedInf = (*i).substr((j + 1), ((*i).size() - (j + 1)));
		j = this->_requestedInf.find("?");
		if (j != std::string::npos)
		{
			this->_queryString = this->_requestedInf.substr((j + 1));
			this->_mapQueryString = this->parseQueryString(this->_queryString);
			this->_requestedInf = this->_requestedInf.substr(0, j);
		}
//		std::cout << "\t\t_location: " << this->_location << " requestedInf: " << this->_requestedInf << std::endl;
	}
//	else
//		std::cout << "urlRequest: " << urlRequest << std::endl;
	if (this->_requestedInf.empty())
	{
		this->_requestedInf = this->_serverConf.getIndex()[0];
//		std::cout << "_requestedInf estáva vazio e agora é: " << this->_requestedInf << std::endl;
	}
	else
	{
//		std::cout << "_requestedInf de tamanho " << this->_requestedInf.size() << std::endl;
		j = 0;
		while (j < this->_requestedInf.size())
		{
			if (!std::isprint(this->_requestedInf[j]))
			{
//				std::cout << "em j: " << j << " é não printável" << std::endl;
				tmp = this->_requestedInf.substr(0, j);
//				std::cout << "com size: " << tmp.size()<< " tmp: " << tmp << std::endl;
				this->_requestedInf.clear();
//				this->_requestedInf = this->_serverConf.getIndex()[0];
				this->_requestedInf = tmp;
				break ;
			}
//			else
//				std::cout << "em j: " << j << " | caract: " << this->_requestedInf[j] << " é printável" << std::endl;
			j++;
		}
	}
	std::cout << "method: " << this->_method << " _location: " << this->_location << " requestedInf: " << this->_requestedInf << " possiu tamanho: " << this->_requestedInf.size();
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
/*
*/
	std::cout << "fim    | splitRequest" << std::endl;
}

void	Request::fixeUrlRequest(std::string & urlRequest)
{
//	std::cout << "início | fixeUrlRequest: " << urlRequest << std::endl;
	if (urlRequest.find("OPTIONS") != std::string::npos)
	{
//		std::cout << "Pré /OPTIONS     urlRequest: " << urlRequest << std::endl;
		urlRequest = this->fixeUrlRequestAux(urlRequest, \
						"OPTIONS", "Access-Control-Request-Method: ");
//		std::cout << "Pós /OPTIONS     urlRequest: " << urlRequest << std::endl;
	}
	if (urlRequest.find("/favicon.ico") != std::string::npos)
	{
//		std::cout << "Pré /favicon.ico urlRequest: " << urlRequest << std::endl;
		urlRequest = this->fixeUrlRequestAux(urlRequest, \
						"/favicon.ico", "Referer: http://");
//		std::cout << "Pós /favicon.ico urlRequest: " << urlRequest << std::endl;
	}
//	std::cout << "fim    | fixeUrlRequest: " << urlRequest << std::endl;
}

std::string Request::fixeUrlRequestAux(std::string & url, \
					const std::string oldValue, const std::string toFind)
{
//	std::cout << "Início | fixeUrlRequestAux: url: |" << url << "| oldValue: |" << oldValue << "| toFind: |" << toFind << "|" << std::endl;
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
				if (this->_header.substr(start, (end - start + 1)).compare(0, 6, "DELETE") == 0)
				{
					this->_method = "DELETE";
//					std::cout << "_method: " << this->_method << std::endl;
				}
				else
				{
					this->_method = this->_header.substr(start, (end - start + 1));
//					std::cout << "_method: " << this->_method << std::endl;
				}
				url = url.substr(oldValue.size() + 1);
//				std::cout << "url:     " << url << std::endl;
			}
			else
			{
				str = this->_header.substr(start, (end - start + 1));
				url.erase(pos, oldValue.size()).insert(pos, str.substr(str.find("/")));
//				std::cout << "url: " << url << std::endl;
			}
		}
		else
			url = "requestError";
	}
	else
		url = "requestError";
//	std::cout << "fim    | fixeUrlRequestAux: url: " << url << std::endl;
	return (url);
}

void	Request::findStartEnd(size_t & start, size_t & end, \
					const std::string toFind, const std::string & place)
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
		}
		else
			start = std::string::npos;
	}
	else
		start = std::string::npos;
}

std::map<std::string, std::string>	Request::parseQueryString(std::string queryString)
{
	std::cout << "início | parseQueryString: " << queryString << std::endl;
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
	std::cout << "fim    | parseQueryString" << std::endl;
	return (mapQueryString);
}

std::string	Request::urlDecoder(const std::string & url)
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

void        Request::reset(void)
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
	this->_queryString.clear();
	this->_mapQueryString.clear();
}

void		Request::printYellow(std::string const& str) const {
	std::cout << "\033[1;33m" << str << "\033[0m" << std::endl;
}
