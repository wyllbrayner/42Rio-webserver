#include "utils/Host.hpp"

namespace net {

Host::Host(void) {}

Host::Host(char const* host) : m_host(host) {}

Host::Host(std::string const& host) : m_host(host) {}

Host::~Host(void) {}

Host& Host::operator=(std::string const& str) {
    m_host = str;
    return (*this);
}

Host& Host::operator=(char const* str) {
    m_host = str;
    return (*this);
}

Host::operator std::string(void) const { return (m_host); }

bool Host::operator==(Host const& rhs) const { return (m_host == rhs.m_host); }

std::vector<std::string> Host::resolve(std::string const& port) const {
    std::vector<std::string> addresses;

    struct addrinfo* result;
    struct addrinfo  hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int status = getaddrinfo(m_host.c_str(), port.c_str(), &hints, &result);
    if (status != 0) {
        LOG_E("getaddrinfo error: " << gai_strerror(status));
        return (addresses);
    }

    for (struct addrinfo* addr = result; addr != NULL; addr = addr->ai_next) {
        char  ip[INET_ADDRSTRLEN];
        void* addrPtr;

        struct sockaddr_in* ipv4 =
            reinterpret_cast<struct sockaddr_in*>(addr->ai_addr);
        addrPtr = &(ipv4->sin_addr);
        inet_ntop(addr->ai_family, addrPtr, ip, sizeof(ip));
        addresses.push_back(ip);
    }

    freeaddrinfo(result);

    return (addresses);
}

// std::vector<std::string> Host::resolve(void) const {
//     std::vector<std::string> addresses;

// struct addrinfo* result;
// struct addrinfo  hints;
// memset(&hints, 0, sizeof(hints));
// hints.ai_family = AF_INET;
// hints.ai_socktype = SOCK_STREAM;
// hints.ai_flags = AI_PASSIVE;

// int status = getaddrinfo(m_host.c_str(), NULL, &hints, &result);
// if (status != 0) {
//     LOG_E("getaddrinfo error: " << gai_strerror(status));
//     return (addresses);
// }

// for (struct addrinfo* addr = result; addr != NULL; addr = addr->ai_next) {
//     char  ip[INET_ADDRSTRLEN];
//     void* addrPtr;

// struct sockaddr_in* ipv4 =
//     reinterpret_cast<struct sockaddr_in*>(addr->ai_addr);
// addrPtr = &(ipv4->sin_addr);
// inet_ntop(addr->ai_family, addrPtr, ip, sizeof(ip));
// addresses.push_back(ip);
// }

// freeaddrinfo(result);

// return (addresses);
// }

// std::vector<std::string> Host::resolve(int port) const {}

} // namespace net