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
        int												totalLength(void);
        bool											isReady(void) const;
		bool											receiveFromClient(int client);
        void											reset(void);
		std::string										totalLengthS(void) const;
        const size_t								&	getContentLength(void) const;
		const ConfigFile							&	getServerConf(void) const;
		const std::string								returnPort(void) const;
		const std::string							&	getMethod(void) const;
		const std::string							&	getLocation(void) const;
		const std::string							&	getRequestedInf(void) const;
		const std::string							&	getContentType(void) const;
		const std::string							&	getBoundary(void) const;
		const std::string							&	returnBody(void) const;
		const std::string							&	getUserAgent(void) const;
		const std::string							&	getHost(void) const;
		const std::string							&	returnHeader(void) const;
		const std::string							&	getQueryStringS(void) const;
		const std::map<std::string, std::string>	&	getMapQueryString(void) const;
		const time_t 								&	getStartTime(void) const;
		void											setStart_time(time_t start_time);

	private:
		int				                                 _fromClient;
		bool											_ready;
		size_t											_contentLength;
		ConfigFile										_serverConf;
		std::string										_header;
		std::string										_host;
		std::string										_userAgent;
		std::string     			    			    _body;
		std::string										_httpMessage;
		std::string										_method;
		std::string										_location;
		std::string										_requestedInf;
		std::string										_contentType;
		std::string										_boundary;
		std::string				                         _delimeter;
		std::string             			            _queryStringS;
		std::map<std::string, std::string>				_mapQueryString;
		int                 			    			checkBytesReceived(ssize_t bytes_received);
		bool					            			splitRequest(std::string header, std::string root);
		bool		    			        			parseRequest(void);
		bool		            						getHeader(std::string const& buffer);
		void											fixeUrlRequest(std::string & urlRequest);
		void											findStartEnd(size_t & start, size_t & end, \
			const std::string toFind, const std::string & place);
		void					            			findContentLength(void);
		void		    			        			findContentType(void);
		void											findHost(void);
		void											findUserAgent(void);
		void											findBoundary(void);
		void		            						getBody(std::string const& buffer, \
			int bytes);
		void			                    			appendTheBody(std::string buffer, \
			const int bytes);
		void											fixebuffer(std::string & buffer);
		void											printYellow(const std::string & str) const;
		std::string										fixeUrlRequestAux(std::string & url, const std::string oldValue, const std::string toFind);
		std::map<std::string, std::string>				parseQueryString(std::string queryString);
		std::string				            			urlDecoder(const std::string & url);
		time_t											_start_time;
};