#include "Client.hpp"

namespace net::test {

Client::Client(std::string port, std::string host) : m_sockFd(-1) {
    try {
        m_addr = new Address(port, host, AF_INET, SOCK_STREAM);
    }
    catch (...) {
        throw GetAddInfoFailureException();
    }
    LOG_D("Created " + toString());
}

Client::~Client(void) {
    if (m_sockFd != -1) { close(); }
    delete m_addr;
}

int Client::getSockFd(void) const { return (m_sockFd); }

int Client::getPort(void) const { return (m_addr->getPort()); }

std::string Client::getHost(void) const { return (m_addr->getHost()); }

sa_family_t Client::getFamily(void) const { return (m_addr->getFamily()); }

int Client::getType(void) const { return (m_addr->getType()); }

sockaddr* Client::getAddress(void) const { return (m_addr->getAddress()); }

socklen_t Client::getLength(void) const { return (m_addr->getLength()); }

socklen_t* Client::getPtrLength(void) { return (m_addr->getPtrLength()); }

void Client::socket(void) {
    if ((m_sockFd = ::socket(m_addr->getFamily(), m_addr->getType(), 0)) < 0) {
        LOG_E(toString() + " failure in ::socket()");
        throw SocketFailureException();
    }
}

void Client::connect(void) {
    if (::connect(m_sockFd, m_addr->getAddress(), m_addr->getLength()) < 0) {
        LOG_E(toString() + " failure in ::connect()");
        throw ConnectFailureException();
    }
}

void Client::close(void) {
    if (m_sockFd == -1) {
        LOG_D(toString() + " is already closed");
        return;
    }
    if (::close(m_sockFd) < 0) {
        LOG_E(toString() + " failure in ::close()");
        throw CloseFailureException();
    }
}

std::string Client::recv(void) {
    char buffer[1024];
    int  nBytes = ::recv(m_sockFd, buffer, 1024, 0);
    if (nBytes == -1) {
        LOG_E(toString() + " failure in ::recv()");
        throw RecvFailureException();
    }
    return (std::string(buffer, nBytes));
}

void Client::send(std::string const& request) {
    if (::send(m_sockFd, request.c_str(), request.size(), 0) < 0) {
        LOG_E(toString() + " failure in ::send()");
        throw SendFailureException();
    }
}

std::string Client::toString(void) const {
    std::ostringstream oss;
    oss << "Test Client " << getPort() << ":" << getHost();
    return (oss.str());
}

char const* Client::SocketFailureException::what(void) const noexcept {
    return ("Test Client ::socket() failure");
}

char const* Client::ConnectFailureException::what(void) const noexcept {
    return ("Test Client ::connect() failure");
}

char const* Client::CloseFailureException::what(void) const noexcept {
    return ("Test Client ::close() failure");
}

char const* Client::GetAddInfoFailureException::what(void) const noexcept {
    return ("Test Client ::getaddinfo() failure");
}

char const* Client::RecvFailureException::what(void) const noexcept {
    return ("Test Client ::recv() failure");
}

char const* Client::SendFailureException::what(void) const noexcept {
    return ("Test Client ::send() failure");
}

Client::Address::Address(std::string port, std::string host, sa_family_t family,
                         int type) {
    struct addrinfo hints;

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = family;
    hints.ai_socktype = type;
    hints.ai_flags = AI_PASSIVE;

    int status = ::getaddrinfo(host.c_str(), port.c_str(), &hints, &m_info);
    if (status != 0) {
        LOG_E("Address " + host + ":" + port +
              ": failure in ::getaddrinfo(): " + gai_strerror(status));
        throw std::exception();
    }

    m_addr = reinterpret_cast<sockaddr_in*>(m_info->ai_addr);
    m_len = m_info->ai_addrlen;

    if (!m_info || !m_addr) {
        LOG_E("Address " + host + ":" + port + ": null pointer in constructor");
        throw std::exception();
    }
}

Client::Address::~Address(void) {
    if (m_info) { ::freeaddrinfo(m_info); }
}

int Client::Address::getPort(void) const { return (ntohs(m_addr->sin_port)); }

std::string Client::Address::getHost(void) const {
    return (inet_ntoa(m_addr->sin_addr));
}

sa_family_t Client::Address::getFamily(void) const {
    return (m_addr->sin_family);
}

int Client::Address::getType(void) const { return (m_info->ai_socktype); }

sockaddr* Client::Address::getAddress(void) const {
    return (reinterpret_cast<sockaddr*>(m_addr));
}

socklen_t Client::Address::getLength(void) const { return (m_len); }

socklen_t* Client::Address::getPtrLength(void) { return (&m_len); }

} // namespace net::test