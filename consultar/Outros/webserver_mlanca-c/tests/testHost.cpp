#include "utils/Host.hpp"

#include <gtest/gtest.h>

TEST(testHost, testConstructor) {
    net::Host host;
    EXPECT_EQ(host, "");
}

TEST(testHost, testAssignment) {
    net::Host host("www.google.com");
    EXPECT_EQ(host, "www.google.com");
}

TEST(testHost, testResolve) {
    net::Host                host("localhost");
    std::vector<std::string> addresses = host.resolve("8080");
    ASSERT_GT(addresses.size(), 0);
    ASSERT_EQ(addresses[0], "127.0.0.1");

    ASSERT_GT(net::Host("www.google.com").resolve("8080").size(), 0);
    ASSERT_EQ(net::Host("").resolve("8080").size(), 0);
}