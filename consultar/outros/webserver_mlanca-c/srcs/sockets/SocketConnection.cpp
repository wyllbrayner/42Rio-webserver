#include "sockets/SocketConnection.hpp"

namespace net {

SocketConnection::SocketConnection(int sockFd, sockaddr_in* addr,
                                   socklen_t addrLen)
    : m_addr(smt::make_shared(new Address(addr, addrLen))), m_sockFd(sockFd) {
    LOG_D("Created " << toString());
}

SocketConnection::~SocketConnection(void) {
    close();
    LOG_D("Destroying " << toString());
}

int SocketConnection::getSockFd(void) const { return (m_sockFd); }

int SocketConnection::getPort(void) const { return (m_addr->getPort()); }

std::string SocketConnection::getHost(void) const {
    return (m_addr->getHost());
}

sa_family_t SocketConnection::getFamily(void) const {
    return (m_addr->getFamily());
}

sockaddr* SocketConnection::getAddress(void) const {
    return (m_addr->getAddress());
}

socklen_t SocketConnection::getLength(void) const {
    return (m_addr->getLength());
}

void SocketConnection::close(void) {
    if (m_sockFd == -1) {
        LOG_D(toString() << " is already closed");
        return;
    }
    if (::close(m_sockFd) == -1) {
        LOG_E(toString() << " failure in ::close()");
        throw CloseFailureException();
    }
    m_sockFd = -1;
}

std::string SocketConnection::recv(void) {
    char buffer[READING_BUFFER << 1];
    int  nBytes = ::recv(m_sockFd, buffer, READING_BUFFER, 0);
    if (nBytes == 0) {
        LOG_E(toString() << " connection closed");
        throw ConnectionClosedException();
    }
    if (nBytes == -1) {
        LOG_E(toString() << " failure in ::recv()");
        throw RecvFailureException();
    }
    return (std::string(buffer, nBytes));
}

void SocketConnection::send(std::string response) {
    if (::send(m_sockFd, response.c_str(), response.size(), 0) < 0) {
        LOG_E(toString() << " failure in ::send()");
        throw SendFailureException();
    }
}

std::string SocketConnection::toString(void) const {
    std::ostringstream oss;
    oss << "Socket Connection " << getPort() << ":" << getHost();
    return (oss.str());
}

char const* SocketConnection::CloseFailureException::what(void) const throw() {
    return ("Socket Connection: failure in ::close().");
}

char const* SocketConnection::SendFailureException::what(void) const throw() {
    return ("Socket Connection: failure in ::send().");
}

char const* SocketConnection::RecvFailureException::what(void) const throw() {
    return ("Socket Connection: failure in ::recv().");
}

char const* SocketConnection::ConnectionClosedException::what(void) const
    throw() {
    return ("Socket Connection: connection closed.");
}

/* Address */
SocketConnection::Address::Address(sockaddr_in* addr, socklen_t addrLen)
    : m_addr(addr), m_len(addrLen) {}

SocketConnection::Address::~Address(void) { delete m_addr; }

int SocketConnection::Address::getPort(void) const {
    return (ntohs(m_addr->sin_port));
}

std::string SocketConnection::Address::getHost(void) const {
    return (inet_ntoa(m_addr->sin_addr));
}

sa_family_t SocketConnection::Address::getFamily(void) const {
    return (getAddress()->sa_family);
}

sockaddr* SocketConnection::Address::getAddress(void) const {
    return (reinterpret_cast<sockaddr*>(m_addr));
}

socklen_t SocketConnection::Address::getLength(void) const { return (m_len); }

std::string SocketConnection::Address::toString(void) const {

    std::ostringstream oss;
    oss << "Connection Address " << getPort() << ":" << getHost();
    return (oss.str());
}
} // namespace net