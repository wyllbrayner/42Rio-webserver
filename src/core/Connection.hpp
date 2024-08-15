#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include "./Server.hpp"
# include <poll.h>
# include <vector>
# include <fcntl.h>
# include <iostream>

class   Connection
{
    public:
        Connection(void);
		Connection(const Connection & copy);
		Connection			&	operator=(const Connection & src);

        ~Connection(void);

        void                    addNewSocket(int socket_fd);
        void                    addClientSocket(int socket);
        void                    addServersSockets(std::vector<Server> const& servers);
        void                    closeConnection(int client);
        void                    closeAllConnections(void);
        const pollfd        &   getFd(int i);
        std::vector<pollfd> &   getPollFd(void);

    private:
        std::vector<pollfd>     _poolAllFd;
};
#endif