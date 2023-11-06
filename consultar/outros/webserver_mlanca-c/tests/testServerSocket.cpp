#include "client/Client.hpp"
#include "sockets/ServerSocket.hpp"
#include "utils/smt.hpp"

#include <gtest/gtest.h>
void setOptions(smt::shared_ptr<net::ServerSocket> sock);

TEST(testServerSocket, testConstructor) {
    ASSERT_NO_THROW(net::ServerSocket(8080, "127.0.0.1"));
}

TEST(testServerSocket, testGetSockFd) {
    smt::shared_ptr<net::ServerSocket> sock(
        new net::ServerSocket(8080, "127.0.0.1"));
    ASSERT_EQ(sock->getSockFd(), -1);
    sock->socket();
    ASSERT_GT(sock->getSockFd(), -1);
}

TEST(testServerSocket, testGetHost) {
    smt::shared_ptr<net::ServerSocket> sock(
        new net::ServerSocket(8080, "127.0.0.1"));
    ASSERT_EQ(sock->getHost(), "127.0.0.1");
}

TEST(testServerSocket, testGetPort) {
    smt::shared_ptr<net::ServerSocket> sock(
        new net::ServerSocket(8080, "127.0.0.1"));
    ASSERT_EQ(sock->getPort(), 8080);
}

TEST(testServerSocket, testGetFamily) {
    smt::shared_ptr<net::ServerSocket> sock(
        new net::ServerSocket(8080, "127.0.0.1"));
    ASSERT_EQ(sock->getFamily(), AF_INET);
}

TEST(testServerSocket, testGetType) {
    smt::shared_ptr<net::ServerSocket> sock(
        new net::ServerSocket(8080, "127.0.0.1"));
    ASSERT_EQ(sock->getType(), SOCK_STREAM);
}

TEST(testServerSocket, testGetAddress) {
    smt::shared_ptr<net::ServerSocket> sock(
        new net::ServerSocket(8080, "127.0.0.1"));
    sockaddr_in* ptrAddr = reinterpret_cast<sockaddr_in*>(sock->getAddress());
    ASSERT_EQ("127.0.0.1", sock->getHost());
    ASSERT_EQ(ptrAddr->sin_port, htons(sock->getPort()));
    ASSERT_EQ(ptrAddr->sin_family, sock->getFamily());
    ASSERT_EQ(SOCK_STREAM, sock->getType());
}

TEST(testServerSocket, testSocket) {
    smt::shared_ptr<net::ServerSocket> sock(
        new net::ServerSocket(8080, "127.0.0.1"));
    ASSERT_EQ(sock->getSockFd(), -1);
    ASSERT_NO_THROW(sock->socket());
    ASSERT_GT(sock->getSockFd(), -1);
}

TEST(testServerSocket, testBind) {
    smt::shared_ptr<net::ServerSocket> sock(
        new net::ServerSocket(8080, "127.0.0.1"));
    ASSERT_NO_THROW(sock->socket());
    ASSERT_NO_THROW(setOptions(sock)) << errno;
    ASSERT_NO_THROW(sock->bind());
}

TEST(testServerSocket, testListen) {
    smt::shared_ptr<net::ServerSocket> sock(
        new net::ServerSocket(8080, "127.0.0.1"));
    ASSERT_NO_THROW(sock->socket());
    ASSERT_NO_THROW(setOptions(sock)) << errno;
    ASSERT_NO_THROW(sock->bind());
    ASSERT_NO_THROW(sock->listen());
}

TEST(testServerSocket, testAccept) {
    smt::shared_ptr<net::ServerSocket> sock(
        new net::ServerSocket(8080, "127.0.0.1"));
    ASSERT_NO_THROW(sock->socket());
    ASSERT_NO_THROW(setOptions(sock)) << errno;
    ASSERT_NO_THROW(sock->bind());
    ASSERT_NO_THROW(sock->listen());
    ASSERT_EQ(sock->getHost(), "127.0.0.1");
    ASSERT_EQ(sock->getPort(), 8080);

    smt::shared_ptr<net::test::Client> client(
        new net::test::Client("8080", "localhost"));
    ASSERT_NO_THROW(client->socket());
    ASSERT_NO_THROW(client->connect());

    int connectFd;
    ASSERT_NO_THROW(connectFd = sock->accept());
    ASSERT_GT(connectFd, -1);
}

TEST(testServerSocket, testClose) {
    smt::shared_ptr<net::ServerSocket> sock(
        new net::ServerSocket(8080, "127.0.0.1"));
    ASSERT_NO_THROW(sock->socket());
    ASSERT_NO_THROW(setOptions(sock)) << errno;
    ASSERT_NO_THROW(sock->bind());
    ASSERT_NO_THROW(sock->close());
}

TEST(testServerSocket, testRecv) {
    smt::shared_ptr<net::ServerSocket> sock(
        new net::ServerSocket(8080, "127.0.0.1"));
    ASSERT_NO_THROW(sock->socket());
    ASSERT_NO_THROW(setOptions(sock)) << errno;
    ASSERT_NO_THROW(sock->bind());
    ASSERT_NO_THROW(sock->listen());

    net::test::Client client("8080", "localhost");
    ASSERT_NO_THROW(client.socket());
    ASSERT_NO_THROW(client.connect());
    ASSERT_NO_THROW(client.send("Hello World!"));

    int connectFd;
    ASSERT_NO_THROW(connectFd = sock->accept());
    ASSERT_GT(connectFd, -1);
    std::string request;
    ASSERT_NO_THROW(request = sock->recv(connectFd));
    ASSERT_EQ(request, "Hello World!");
}

TEST(testServerSocket, testSend) {
    smt::shared_ptr<net::ServerSocket> sock(
        new net::ServerSocket(8080, "127.0.0.1"));
    ASSERT_NO_THROW(sock->socket());
    ASSERT_NO_THROW(setOptions(sock)) << errno;
    ASSERT_NO_THROW(sock->bind());
    ASSERT_NO_THROW(sock->listen());

    smt::shared_ptr<net::test::Client> client(
        new net::test::Client("8080", "localhost"));
    ASSERT_NO_THROW(client->socket());
    ASSERT_NO_THROW(client->connect());
    ASSERT_NO_THROW(client->send("Hello World!"));

    int connectFd;
    ASSERT_NO_THROW(connectFd = sock->accept());
    ASSERT_GT(connectFd, -1);
    std::string request;
    ASSERT_NO_THROW(request = sock->recv(connectFd));
    ASSERT_EQ(request, "Hello World!");
    ASSERT_NO_THROW(sock->send(connectFd, request));
    std::string response;
    ASSERT_NO_THROW(response = client->recv());
    ASSERT_EQ(response, "Hello World!");
}

void setOptions(smt::shared_ptr<net::ServerSocket> sock) {

    int const enable = 1;
    sock->setsockopt(SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    sock->setsockopt(SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int));
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    sock->setsockopt(SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval));
}