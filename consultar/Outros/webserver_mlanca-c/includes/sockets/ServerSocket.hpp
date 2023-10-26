#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

#include "server/Server.hpp"
#include "sockets/SocketConnection.hpp"
#include "utils/Logger.hpp"
#include "utils/smt.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <map>
#include <netdb.h>
#include <sstream>
#include <sys/socket.h>

namespace net {

class SocketConnection;

class ServerSocket {

    public:

        ServerSocket(int port, std::string const& host);
        ~ServerSocket(void);

        int         getSockFd(void) const;
        int         getPort(void) const;
        std::string getHost(void) const;
        sa_family_t getFamily(void) const;
        int         getType(void) const;
        sockaddr*   getAddress(void) const;
        socklen_t   getLength(void) const;

        smt::shared_ptr<SocketConnection> getConnection(int connectFd);
        std::map< int, smt::shared_ptr<SocketConnection> > getConnections(void);

        void socket(void);
        void bind(void);
        void listen(void);
        void setsockopt(int level, int optname, void const* optval,
                        socklen_t optlen);
        int  accept(void);
        void close(void);
        void close(int connectFd);

        std::string recv(int connectFd);
        void        send(int connectFd, std::string const& response);

        struct SocketFailureException : public std::exception {
                char const* what(void) const throw();
        };

        struct BindFailureException : public std::exception {
                char const* what(void) const throw();
        };

        struct ListenFailureException : public std::exception {
                char const* what(void) const throw();
        };

        struct SetOptFailureException : public std::exception {
                char const* what(void) const throw();
        };

        struct AcceptFailureException : public std::exception {
                char const* what(void) const throw();
        };

        struct CloseFailureException : public std::exception {
                char const* what(void) const throw();
        };

        struct SendFailureException : public std::exception {
                char const* what(void) const throw();
        };

        struct RecvFailureException : public std::exception {
                char const* what(void) const throw();
        };

        struct NoSuchConnectionException : public std::exception {
                char const* what(void) const throw();
        };

    private:

        class Address {

            public:

                Address(int port, std::string host);
                ~Address(void);

                int         getPort(void) const;
                std::string getHost(void) const;
                sa_family_t getFamily(void) const;
                int         getType(void) const;
                sockaddr*   getAddress(void) const;
                socklen_t   getLength(void) const;
                socklen_t*  getPtrLength(void);

                struct GetAddInfoFailureException : public std::exception {
                        char const* what(void) const throw();
                };

            private:

                std::string toString(void) const;

                sockaddr_in* m_addr;
                socklen_t    m_len;
        };

        std::string toString(void) const;

        int                      m_sockFd;
        smt::shared_ptr<Address> m_addr;

        std::map< int, smt::shared_ptr<SocketConnection> > m_connections;
};

} // namespace net

#endif /* SERVER_SOCKET_HPP */