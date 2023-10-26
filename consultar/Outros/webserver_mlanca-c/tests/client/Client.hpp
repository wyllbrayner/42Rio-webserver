#ifndef NET_TEST_CLIENT_HPP
#define NET_TEST_CLIENT_HPP

#include "utils/Logger.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <sstream>
#include <string>
#include <unistd.h>

namespace net::test {

class Client {
    public:

        Client(std::string port, std::string host);
        ~Client(void);

        int         getSockFd(void) const;
        int         getPort(void) const;
        std::string getHost(void) const;
        sa_family_t getFamily(void) const;
        int         getType(void) const;
        sockaddr*   getAddress(void) const;
        socklen_t   getLength(void) const;
        socklen_t*  getPtrLength(void);

        void socket(void);
        void connect(void);
        void close(void);

        std::string recv(void);
        void        send(std::string const& request);

        struct SocketFailureException : public std::exception {
                char const* what(void) const throw();
        };

        struct ConnectFailureException : public std::exception {
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

        struct GetAddInfoFailureException : public std::exception {
                char const* what(void) const throw();
        };

    private:

        std::string toString(void) const;

        class Address {

            public:

                Address(std::string port, std::string host, sa_family_t family,
                        int type);
                ~Address(void);

                int         getPort(void) const;
                std::string getHost(void) const;
                sa_family_t getFamily(void) const;
                int         getType(void) const;
                sockaddr*   getAddress(void) const;
                socklen_t   getLength(void) const;
                socklen_t*  getPtrLength(void);

            private:

                addrinfo*    m_info;
                sockaddr_in* m_addr;
                socklen_t    m_len;
        };

        Address* m_addr;
        int      m_sockFd;
};

} // namespace net::test

#endif /* NET_TEST_CLIENT_HPP */