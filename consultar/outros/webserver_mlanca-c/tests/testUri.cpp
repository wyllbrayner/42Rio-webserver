#include "http/Uri.hpp"
#include "utils/Logger.hpp"
#include "utils/smt.hpp"

#include <gtest/gtest.h>

TEST(testUri, testConstructor) {

    smt::shared_ptr<http::Uri> uri;

    // testing uri in absolute-form without query
    uri = smt::make_shared(
        new http::Uri("http://www.example.org/pub/WWW/TheProject.html"));
    EXPECT_EQ(uri->getScheme(), "http") << uri->getScheme();
    EXPECT_EQ(uri->getHost(), "www.example.org") << uri->getHost();
    EXPECT_TRUE(uri->getPort().empty()) << uri->getPort();
    EXPECT_EQ(uri->getPath(), "/pub/WWW/TheProject.html") << uri->getPath();
    EXPECT_TRUE(uri->getQuery().empty()) << uri->getQuery();

    // testing uri in absolute-form with query
    uri = smt::make_shared(new http::Uri(
        "http://www.example.org/pub/WWW/TheProject.html?tag=networking"));
    EXPECT_EQ(uri->getScheme(), "http") << uri->getScheme();
    EXPECT_EQ(uri->getHost(), "www.example.org") << uri->getHost();
    EXPECT_TRUE(uri->getPort().empty()) << uri->getPort();
    EXPECT_EQ(uri->getPath(), "/pub/WWW/TheProject.html") << uri->getPath();
    EXPECT_EQ(uri->getQuery(), "tag=networking") << uri->getQuery();

    // testing uri in origin-form without query
    uri = smt::make_shared(new http::Uri("/where"));
    EXPECT_TRUE(uri->getScheme().empty()) << uri->getScheme();
    EXPECT_TRUE(uri->getPort().empty()) << uri->getPort();
    EXPECT_TRUE(uri->getHost().empty()) << uri->getHost();
    EXPECT_EQ(uri->getPath(), "/where") << uri->getPath();
    EXPECT_TRUE(uri->getQuery().empty()) << uri->getQuery();

    // testing uri in origin-form with query
    uri = smt::make_shared(new http::Uri("/where?q=now"));
    EXPECT_TRUE(uri->getScheme().empty()) << uri->getScheme();
    EXPECT_TRUE(uri->getPort().empty()) << uri->getPort();
    EXPECT_TRUE(uri->getHost().empty()) << uri->getHost();
    EXPECT_EQ(uri->getPath(), "/where") << uri->getPath();
    EXPECT_EQ(uri->getQuery(), "q=now") << uri->getQuery();

    // testing uri in absolute-form without query
    uri = smt::make_shared(new http::Uri("localhost:3000/cgi/python/me.py"));
    EXPECT_TRUE(uri->getScheme().empty()) << uri->getScheme();
    EXPECT_EQ(uri->getHost(), "localhost") << uri->getHost();
    EXPECT_EQ(uri->getPort(), "3000") << uri->getPort();
    EXPECT_EQ(uri->getPath(), "/cgi/python/me.py") << uri->getPath();
    EXPECT_TRUE(uri->getQuery().empty()) << uri->getQuery();

    // testing uri in absolute-form with query
    uri = smt::make_shared(new http::Uri(
        "localhost:3000/cgi/python/me.py?tag=networking&tag=web"));
    EXPECT_TRUE(uri->getScheme().empty()) << uri->getScheme();
    EXPECT_EQ(uri->getHost(), "localhost") << uri->getHost();
    EXPECT_EQ(uri->getPort(), "3000") << uri->getPort();
    EXPECT_EQ(uri->getPath(), "/cgi/python/me.py") << uri->getPath();
    EXPECT_EQ(uri->getQuery(), "tag=networking&tag=web") << uri->getQuery();

    // testing uri in absolute-form with pathInfo
    uri = smt::make_shared(
        new http::Uri("localhost:3000/cgi/python/me.py/where/something/else"));
    EXPECT_TRUE(uri->getScheme().empty()) << uri->getScheme();
    EXPECT_EQ(uri->getHost(), "localhost") << uri->getHost();
    EXPECT_EQ(uri->getPort(), "3000") << uri->getPort();
    EXPECT_EQ(uri->getPath(), "/cgi/python/me.py/where/something/else")
        << uri->getPath();
    EXPECT_TRUE(uri->getQuery().empty()) << uri->getQuery();

    // testing uri in absolute-form with pathInfo and query
    uri = smt::make_shared(new http::Uri(
        "localhost:3000/cgi/python/me.py/where?q=now&tag=networking&tag=web"));
    EXPECT_TRUE(uri->getScheme().empty()) << uri->getScheme();
    EXPECT_EQ(uri->getHost(), "localhost") << uri->getHost();
    EXPECT_EQ(uri->getPort(), "3000") << uri->getPort();
    EXPECT_EQ(uri->getPath(), "/cgi/python/me.py/where") << uri->getPath();
    EXPECT_EQ(uri->getQuery(), "q=now&tag=networking&tag=web")
        << uri->getQuery();
}

TEST(testUri, testRequestFailure) {
    http::Uri uri("/cgi-bin/python/me.py");
    EXPECT_EQ(uri.getScheme(), "") << uri.getScheme();
    EXPECT_EQ(uri.getHost(), "") << uri.getHost();
    EXPECT_EQ(uri.getPort(), "") << uri.getPort();
    EXPECT_EQ(uri.getPath(), "/cgi-bin/python/me.py") << uri.getPath();
    EXPECT_EQ(uri.getQuery(), "") << uri.getQuery();
}