#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string.h>
# include <iostream>
# include <sstream>

class   Response {
    public:
        Response();
        ~Response();

        void createHTTPHeader2(std::string statusCode, std::string contentType);
        void createHTTPHeader(int statusCode, std::string contentType, int contentLength);
        void send();
        void setBody(std::string body);
        std::string toString(int number);
        std::string httpMessage;
        void        processFileForHTTPResponse(std::string &file, int statusCode);
        void        processFileForHTTPResponse2(std::stringstream &file, std::string statusCode);
    
    private:
        std::string _body;
        std::string _header;
        int         _statusCode;
};

#endif