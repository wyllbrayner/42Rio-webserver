#pragma	once

# include <fstream>
# include <string>
# include <iostream> //confirmar se será necessário para a entrega
# include <sstream>
# include <map>
# include <sys/socket.h>
# include <string.h>
# include <string> //to_string()
# include <algorithm> //for search
# include "./../Error.hpp"
# include "../Utils.hpp"
# include "../config/ConfigFile.hpp"

# define BUFFER_SIZE 4096

class	Request
{
	public:
		Request(int newClient, ConfigFile _configFile);
		~Request(void);

		const std::string	&	getMethod(void) const;
		const std::string	&	getLocation(void) const;
		const std::string	&	getRequestedInf(void) const;
		const std::string	&	getContentType(void) const;
		const ConfigFile	&	getServerConf(void) const;
        int						getContentLength(void) const;
		bool		            receiveFromClient(int client);
        bool                    isReady(void) const;
        void                    reset(void);
		const std::map<std::string, std::string> &	getQueryString(void) const;

	private:
        int                                 _fromClient;
        bool                                _ready;
		size_t								_contentLength;
    	ConfigFile							_serverConf;
		std::string							_header;
        std::string     			        _body;
		std::string							_httpMessage;
		std::string							_method;
		std::string							_location;
		std::string							_requestedInf;
		std::string							_contentType;
        std::string                         _delimeter;
		std::map<std::string, std::string>	_queryString;

//		void		            parseRequest(void);
//		void		            splitRequest(std::string header, std::string root);
		bool		            parseRequest(void);
		bool		            splitRequest(std::string header, std::string root);
		void					fixeUrlRequest(std::string & urlRequest);
		std::string				fixeUrlRequestAux(std::string & url, \
								const std::string oldValue, const std::string toFind);
		void					findStartEnd(size_t & start, size_t & end, \
								const std::string toFind, const std::string & place);
		void		            parseQueryString(std::string queryString);
		std::string	            urlDecoder(const std::string & url);
        int                     checkBytesReceived(ssize_t bytes_received);
		bool		            getHeader(std::string const& buffer);
		void		            getContentLength(void);
		void		            getContentType(void);
		void		            getBody(std::string const& buffer, int bytes);
        void                    appendTheBody(std::string const& buffer, int bytes);
		void					printYellow(std::string const& str) const;
};
