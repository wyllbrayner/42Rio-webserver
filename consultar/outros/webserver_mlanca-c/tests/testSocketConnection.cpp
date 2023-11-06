#include "client/Client.hpp"
#include "sockets/ServerSocket.hpp"
#include "sockets/SocketConnection.hpp"
#include "utils/smt.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <gtest/gtest.h>
#include <netdb.h>
#include <string>

class testSocketConnection : public ::testing::Test {

        void SetUp(void) {

            // creating listener socket binded to 8080:localhost
            // std::cout << "Creating Listenner" << std::endl;
            listener =
                smt::make_shared(new net::ServerSocket(8080, "127.0.0.1"));
            ASSERT_NO_THROW(listener->socket());

            int const enable = 1;
            listener->setsockopt(SOL_SOCKET, SO_REUSEADDR, &enable,
                                 sizeof(int));
            listener->setsockopt(SOL_SOCKET, SO_REUSEPORT, &enable,
                                 sizeof(int));
            struct timeval timeout;
            timeout.tv_sec = 1;
            timeout.tv_usec = 0;
            listener->setsockopt(SOL_SOCKET, SO_RCVTIMEO, &timeout,
                                 sizeof(struct timeval));

            ASSERT_NO_THROW(listener->bind());
            ASSERT_NO_THROW(listener->listen());

            // creating client that will try and connect with 8080:localhost
            client =
                smt::make_shared(new net::test::Client("8080", "localhost"));
            ASSERT_NO_THROW(client->socket());
            ASSERT_NO_THROW(client->connect());

            // accepting connection
            int fd;
            ASSERT_NO_THROW(fd = listener->accept());

            // getting connector
            sock = listener->getConnection(fd);
        }

        void TearDown(void) {}

    protected:

        smt::shared_ptr<net::ServerSocket>     listener;
        smt::shared_ptr<net::test::Client>     client;
        smt::shared_ptr<net::SocketConnection> sock;
};

TEST_F(testSocketConnection, testGetSockfd) {
    ASSERT_GT(sock->getSockFd(), -1);
}

TEST_F(testSocketConnection, testGetPort) { ASSERT_GT(sock->getPort(), 1); }

TEST_F(testSocketConnection, testGetHost) {
    ASSERT_EQ(sock->getHost(), "127.0.0.1");
}

TEST_F(testSocketConnection, testGetFamily) {
    ASSERT_EQ(sock->getFamily(), AF_INET);
}

TEST_F(testSocketConnection, testGetAddress) {
    ASSERT_NE(sock->getAddress(), nullptr);
}

TEST_F(testSocketConnection, testGetLength) {
    ASSERT_EQ(sock->getLength(), sizeof(sockaddr_in));
}

TEST_F(testSocketConnection, testClose) { ASSERT_NO_THROW(sock->close()); }

TEST_F(testSocketConnection, testRecv) {
    ASSERT_NO_THROW(client->send("Hello World!"));
    std::string request;
    ASSERT_NO_THROW(request = sock->recv());
    ASSERT_EQ(request, "Hello World!");
}

TEST_F(testSocketConnection, testSend) {
    ASSERT_NO_THROW(sock->send("Hello World!"));
    std::string response;
    ASSERT_NO_THROW(response = client->recv());
    ASSERT_EQ(response, "Hello World!");
}