#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Request.hpp"
# include "Response.hpp"

class Client
{
    public:
        Client(const Request &request, Response & response);
        ~Client();

        void        choiceMethod();
        std::string getMethod(void);
        void        postMethod(void);
        void        deleteMethod(void);

    private:
        Client(const Client &client);
        Client &operator=(const Client &Client);
        Request const&      _request;
        Response &     _response;
        // std::string         _response;
        int                 _statusCode;
};

#endif