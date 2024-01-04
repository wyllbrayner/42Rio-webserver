#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <dirent.h>

#include "../Utils.hpp"
#include "Request.hpp"
#include "Response.hpp"

class Client
{
    public:
        Client(const Request &request, Response & response);
        ~Client();

        int                     getStatusCode(void);
        void                    choiceMethod(void);
        void                    postMethod(void);
        void                    deleteMethod(void);
        std::string             getMethod(void);
//        std::string             readFile(std::string name); 
        const std::string   &   getCode(void) const;

    private:
        Client(const Client &client);
        Client &operator=(const Client &Client);
        Request const&      _request;
        Response &          _response;
        int                 _statusCode;
        std::string         _code;
        std::string	        fileRequested(void);
        void                searchErrorFile(std::string & fileRequested, std::string errorCode);
        void                buildDeleteFile(const std::string & path, const std::string & idValue);
        void                buildAutoindexPage(std::string & path);
        void                buildHeadOfPage(std::string & page, const std::string & delimeter, std::string status, const std::string & path);
        void                buildDefaultErrorPage(std::string & page, \
        const std::string & errorCode);
        void                buildDefaultPage(std::string & page);
};

#endif