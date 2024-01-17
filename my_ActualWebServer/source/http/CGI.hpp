#pragma once

#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <string.h>

#include "Client.hpp"
#include "Request.hpp"

#define BUFFER_SIZE_CGI 64 * 1024  // 64 KB
#define TIME_LIMIT 3

class   CGI
{
    private:
        int                 _pid;
        int                 _requestFD[2];
        bool                _isActive;
        pid_t               _cgi_pid;
        time_t              _start_time;
        std::string         _path;
        std::string         _response;
        const Request   &   _request;
        std::vector<char*>  _env;
        int                 executeGET(void);
        int                executePOST(void);
        void                initEnvGET(std::string queryString);
        void                initEnvPOST(std::string queryString);
        int                readFD(int fd);
        bool                writeFD(std::string body);
        void                routineCheck(int bytes, int pipefd);
    
    public:
        CGI(std::string path, const Request & request);
/*
		CGI(const CGI & copy);
        CGI				&	operator=(const CGI & src);
*/
        ~CGI(void);
        std::string         getBody(void) const;
        bool                executeCGI();
        
};