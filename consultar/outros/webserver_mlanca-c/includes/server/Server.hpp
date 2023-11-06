#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include "sockets/ServerSocket.hpp"
#include "utils/smt.hpp"

#include <cstring>
#include <map>
#include <string>
#include <sys/epoll.h>
#include <unistd.h>

namespace net {
class ServerSocket;
}

namespace webserv {

#define BACKLOG 42

#define READING_BUFFER 8192

#define EP_MAX_EVENTS 42
#define EP_TIMEOUT    5000

class Server {

    private:

        typedef enum s_state { Ready, Started, Running, Stopped } t_state;

    public:

        ~Server(void);

        static smt::shared_ptr<Server> getInstance(void);

        static void start(void);
        static void run(void);
        static void stop(void);

        struct EpollCreateException : public std::exception {
                char const* what(void) const throw();
        };

        struct EpollAddException : public std::exception {
                char const* what(void) const throw();
        };

        struct EpollRemoveException : public std::exception {
                char const* what(void) const throw();
        };

        struct EpollWaitException : public std::exception {
                char const* what(void) const throw();
        };

    private:

        void startServer(void);
        void runServer(void);
        void stopServer(void);

        Server(void);

        void startSocket(int port, std::string const& host);
        void epollAdd(int fd, int events = EPOLLIN);
        void epollRemove(int fd);

        t_state m_state;
        int     m_epollFd;

        std::map< int, smt::shared_ptr<net::ServerSocket> > m_sockets;
};

} // namespace webserv

#endif /* WEBSERV_SERVER_HPP */