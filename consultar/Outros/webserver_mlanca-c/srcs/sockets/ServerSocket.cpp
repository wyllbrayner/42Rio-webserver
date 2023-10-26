#include "sockets/ServerSocket.hpp"

namespace net {

ServerSocket::ServerSocket(int port, std::string const& host)
    : m_sockFd(-1), m_addr(smt::make_shared(new Address(port, host))) {
    LOG_D("Created " << toString());
}

ServerSocket::~ServerSocket(void) {
    close();
    LOG_D("Destroying " << toString());
}

int ServerSocket::getSockFd(void) const { return (m_sockFd); }

int ServerSocket::getPort(void) const { return (m_addr->getPort()); }

std::string ServerSocket::getHost(void) const { return (m_addr->getHost()); }

sa_family_t ServerSocket::getFamily(void) const {
    return (m_addr->getFamily());
}

int ServerSocket::getType(void) const { return (m_addr->getType()); }

sockaddr* ServerSocket::getAddress(void) const {
    return (m_addr->getAddress());
}

socklen_t ServerSocket::getLength(void) const { return (m_addr->getLength()); }

void ServerSocket::socket(void) {
    if ((m_sockFd = ::socket(m_addr->getFamily(), m_addr->getType(), 0)) < 0) {
        LOG_E(toString() + " failure in ::socket()");
        throw SocketFailureException();
    }
}

smt::shared_ptr<SocketConnection> ServerSocket::getConnection(int connectFd) {

    std::map<int, smt::shared_ptr<SocketConnection> >::const_iterator it;
    it = m_connections.find(connectFd);
    if (it == m_connections.end()) { throw NoSuchConnectionException(); }
    return (it->second);
}

std::map< int, smt::shared_ptr<SocketConnection> >
    ServerSocket::getConnections(void) {
    if (m_connections.empty()) { LOG_E(toString() + " has no connections"); }
    return (m_connections);
}

void ServerSocket::bind(void) {
    if (::bind(m_sockFd, m_addr->getAddress(), m_addr->getLength()) == -1) {
        LOG_E(toString() + " failure in ::bind()");
        throw BindFailureException();
    }
}

void ServerSocket::listen(void) {
    if (::listen(m_sockFd, BACKLOG) == -1) {
        LOG_E(toString() + " failure in ::listen()");
        throw ListenFailureException();
    }
}

void ServerSocket::setsockopt(int level, int optname, void const* optval,
                              socklen_t optlen) {
    if (::setsockopt(m_sockFd, level, optname, optval, optlen) == -1) {
        LOG_E(toString() + " failure in ::setsockopt()");
        throw SetOptFailureException();
    }
}

int ServerSocket::accept(void) {

    sockaddr* connectAddr = new sockaddr;
    socklen_t len = sizeof(connectAddr);

    int connectFd = ::accept(m_sockFd, connectAddr, &len);

    if (connectFd < 0) {
        LOG_E(toString() + " failure in ::accept()");
        throw(AcceptFailureException());
    }

    smt::shared_ptr< SocketConnection > sockConnect(new SocketConnection(
        connectFd, reinterpret_cast<sockaddr_in*>(connectAddr), len));

    // m_connections[connectFd] = sockConnect;
    m_connections.insert(std::make_pair(connectFd, sockConnect));
    return (connectFd);
}

void ServerSocket::close(void) {

    if (m_sockFd == -1) {
        LOG_D(toString() + " is already closed");
        return;
    }

    if (::close(m_sockFd) == -1) {
        LOG_E(toString() + " failure in ::close()");
        throw CloseFailureException();
    }
    m_sockFd = -1;
}

void ServerSocket::close(int connectFd) {

    std::map<int, smt::shared_ptr<SocketConnection> >::iterator it;
    it = m_connections.find(connectFd);

    if (it == m_connections.end()) {
        LOG_E(toString() + " failure in ::close(): no such connection");
        throw(NoSuchConnectionException());
    }

    m_connections.erase(it);
}

std::string ServerSocket::recv(int connectFd) {

    std::map<int, smt::shared_ptr<SocketConnection> >::iterator it;
    it = m_connections.find(connectFd);

    if (it == m_connections.end()) {
        LOG_E(toString() + " failure in ::recv(): no such connection");
        throw(NoSuchConnectionException());
    }

    std::string request;
    try {
        request = (*it).second->recv();
    }
    catch (SocketConnection::ConnectionClosedException const& e) {
        LOG_E(toString() + " failure in ::recv() " + e.what());
        m_connections.erase(it);
    }
    catch (SocketConnection::RecvFailureException const& e) {
        LOG_E(toString() + " failure in ::recv(): " + e.what());
        m_connections.erase(it);
    }
    return (request);
}

void ServerSocket::send(int connectFd, std::string const& response) {

    std::map<int, smt::shared_ptr<SocketConnection> >::iterator it;
    it = m_connections.find(connectFd);

    if (it == m_connections.end()) {
        LOG_E(toString() + " failure in ::send(): no such connection");
        throw(NoSuchConnectionException());
    }

    try {
        (*it).second->send(response);
    }
    catch (SocketConnection::SendFailureException& e) {
        LOG_E(toString() + " failure in ::send(): " + e.what());
        m_connections.erase(it);
    }
}

std::string ServerSocket::toString(void) const {
    std::ostringstream oss;
    oss << "Server Socket " << getPort() << ":" << getHost();
    return (oss.str());
}

char const* ServerSocket::SocketFailureException::what(void) const throw() {
    return ("Server Socket: failure in ::socket().");
}

char const* ServerSocket::BindFailureException::what(void) const throw() {
    return ("Server Socket: failure in ::bind().");
}

char const* ServerSocket::ListenFailureException::what(void) const throw() {
    return ("Server Socket: failure in ::listen().");
}

char const* ServerSocket::SetOptFailureException::what(void) const throw() {
    return ("Server Socket: failure in ::setsockopt().");
}

char const* ServerSocket::AcceptFailureException::what(void) const throw() {
    return ("Server Socket: failure in ::accept().");
}

char const* ServerSocket::CloseFailureException::what(void) const throw() {
    return ("Server Socket: failure in ::close().");
}

char const* ServerSocket::SendFailureException::what(void) const throw() {
    return ("Server Socket: failure in ::send().");
}

char const* ServerSocket::RecvFailureException::what(void) const throw() {
    return ("Server Socket: failure in ::recv().");
}

char const* ServerSocket::NoSuchConnectionException::what(void) const throw() {
    return ("Server Socket: no such connection.");
}

/* Address */
ServerSocket::Address::Address(int port, std::string host)
    : m_addr(new sockaddr_in), m_len(sizeof(*m_addr)) {
    m_addr->sin_family = AF_INET;
    m_addr->sin_port = htons(port);
    m_addr->sin_addr.s_addr = inet_addr(host.c_str());
}

ServerSocket::Address::~Address(void) { delete m_addr; }

int ServerSocket::Address::getPort(void) const {
    return (ntohs(m_addr->sin_port));
}

std::string ServerSocket::Address::getHost(void) const {
    return (inet_ntoa(m_addr->sin_addr));
}

sa_family_t ServerSocket::Address::getFamily(void) const {
    return (m_addr->sin_family);
}

int ServerSocket::Address::getType(void) const { return (SOCK_STREAM); }

sockaddr* ServerSocket::Address::getAddress(void) const {
    return (reinterpret_cast<sockaddr*>(m_addr));
}

socklen_t ServerSocket::Address::getLength(void) const { return (m_len); }

std::string ServerSocket::Address::toString(void) const {

    std::ostringstream oss;
    oss << "Server Address " << getPort() << ":" << getHost();
    return (oss.str());
}

} // namespace net