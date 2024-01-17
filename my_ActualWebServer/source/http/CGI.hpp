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
#define TIME_LIMIT 30

class CGI
{
    private:
        std::string         _path; 
        int                 _pid;
        std::vector<char *> _env;
        Request const&      _request;
        std::string         _response; //page 
        pid_t               _cgi_pid;
        int                 _requestFD[2];
        time_t              _start_time;
        bool                _isActive;

    public:
        CGI(std::string path, Request const & request);
        ~CGI();
    
        void executeGET(void);
        void executePOST(void);

        void initEnvGET(std::string queryString);
        void initEnvPOST(std::string queryString);

        void routineCheck(void);

        void readFD(int fd);
        bool writeFD(std::string body);

        std::string getBody(void) const;
        

};