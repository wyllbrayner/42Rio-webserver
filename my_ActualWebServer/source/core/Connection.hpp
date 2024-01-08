#ifndef CONNECTION_HPP
# define CONNECTION_HPP

//# include "Webserv.hpp"
# include "./Server.hpp"
# include <poll.h>
# include <vector>
# include <fcntl.h>
# include <iostream>

class Connection
{
    public:
        Connection();
        ~Connection();

        void    addNewSocket(int socket_fd);
        void    addClientSocket(int socket);
        void	addServersSockets(std::vector<Server> const& servers);
        void    closeConnection(int client);
        void	closeAllConnections(void);

        std::vector<pollfd> &   getPollFd(void);
        const pollfd        &   getFd(int i);

    private:
        std::vector<pollfd> poolAllFd;
};

#endif