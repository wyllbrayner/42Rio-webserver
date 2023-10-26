#ifndef SOCKET_CONNECTION_HPP
#define SOCKET_CONNECTION_HPP

#include "server/Server.hpp"
#include "utils/Logger.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

namespace net {

class SocketConnection {

    public:

        SocketConnection(int sockFd, sockaddr_in* addr, socklen_t addrLen);
        ~SocketConnection(void);

        int         getSockFd(void) const;
        int         getPort(void) const;
        std::string getHost(void) const;
        sa_family_t getFamily(void) const;
        sockaddr*   getAddress(void) const;
        socklen_t   getLength(void) const;

        void close(void);

        std::string recv(void);
        void        send(std::string response);

        struct CloseFailureException : public std::exception {
                char const* what(void) const throw();
        };

        struct SendFailureException : public std::exception {
                char const* what(void) const throw();
        };

        struct RecvFailureException : public std::exception {
                char const* what(void) const throw();
        };

        struct ConnectionClosedException : public std::exception {
                char const* what(void) const throw();
        };

    private:

        class Address {

            public:

                Address(sockaddr_in* addr, socklen_t addrLen);
                ~Address(void);

                int         getPort(void) const;
                std::string getHost(void) const;
                sa_family_t getFamily(void) const;
                sockaddr*   getAddress(void) const;
                socklen_t   getLength(void) const;

            private:

                std::string toString(void) const;

                sockaddr_in* m_addr;
                socklen_t    m_len;
        };

        std::string toString(void) const;

        smt::shared_ptr<Address> m_addr;
        int                      m_sockFd;
};

} // namespace net

#endif /* SOCKET_CONNECTION_HPP */
