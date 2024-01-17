#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <vector>

# include "./Server.hpp"
# include "./Connection.hpp"
# include "../config/ParserServer.hpp"
# include "../http/Client.hpp"
# include "../http/Request.hpp"
# include "../Utils.hpp"

class   Webserv
{
    private:
        int                     _nbrServers;
        std::vector<Server>     _servers;
        Connection              _conn;
        std::vector<Request>    _requests;
        void                    start(void);
        bool                    updateStatusPoll(void);
        bool                    isAbleToRead(int i);
        bool                    isAbleToWrite(int i);
        bool                    isPollError(int i);
        bool                    isRequestFromServer(int i);
        void                    openNewConnection(int i);
        void                    readDataClient(const int & i);
        void                    sendDataClient(const int & i);

    public:
        Webserv(const std::vector<Server> & newServers);
		Webserv(const Webserv & copy);
		Webserv				&	operator=(const Webserv & src);
        ~Webserv(void);
};
#endif