#pragma once

#include <string>

class CGI
{
    private:
        CGI(const CGI& cgi);
        CGI& operator=(const CGI& cgi);
        

    public:
        CGI(std::string path);

};