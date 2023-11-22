#pragma	once

# include <fstream>
# include <string>
# include <iostream> //confirmar se será necessário para a entrega
# include <sstream>
# include <map>
# include "./../config/Error.hpp"
# include "./../config/Utils.hpp"
# include <sys/socket.h>
# include <string.h>

class	Request
{
	private:
		std::string							_httpMessage;
		std::string							_method;
		std::string							_location;
		std::string							_requestedInf;
		std::string							_contentType;
		std::map<std::string, std::string>	_queryString;
		Request(const Request& copy);
		Request	&operator=(const Request &src);

		void		splitRequest(std::string & fullRequest, size_t & pos);
		void		parseQueryString(std::string queryString);
		std::string	urlDecoder(const std::string & url);

	public:
		Request(void);
		~Request(void);

		int			receiveFromClient(int client);
		const std::string &							getMethod(void) const;
		const std::string &							getLocation(void) const;
		const std::string &							getRequestedInf(void) const;
		const std::string &							getContentType(void) const;
		const std::map<std::string, std::string> &	getQueryString(void) const;

		void					parseRequest();
};
