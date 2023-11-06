#include "http/Request.hpp"

#include <gtest/gtest.h>

TEST(testRequest, testConstructor) {
    std::string   req = "GET / HTTP/1.1\r\nHost: x\r\nContent-Length: "
                        "14\r\n\r\nThis is a body";
    http::Request request(req);
    EXPECT_EQ(request.getMethod(), "GET");
    EXPECT_EQ(request.getVersion(), "HTTP/1.1");
    EXPECT_EQ(request.getHeader("Host"), "x");
    EXPECT_EQ(request.getHeader("Content-Length"), "14");
    EXPECT_EQ(request.getBody(), "This is a body");

    ASSERT_THROW(http::Request(""), http::Request::MalformedRequestException);

    ASSERT_THROW(http::Request("GET HTTP/1.1\r\nHost: x\r\nContent-Length: "
                               "15\r\n\r\nThis is a body"),
                 http::Request::MalformedRequestException);
    ASSERT_THROW(http::Request("GET / \r\nHost: x\r\nContent-Length: "
                               "15\r\n\r\nThis is a body"),
                 http::Request::MalformedRequestException);

    ASSERT_THROW(
        http::Request(
            "GET somthing somethingelseHTTP/1.1\r\nHost: x\r\nContent-Length: "
            "15\r\n\r\nThis is a body"),
        http::Request::MalformedRequestException);

    ASSERT_THROW(
        http::Request("somthing GET / HTTP/1.1\r\nHost: x\r\nContent-Length: "
                      "15\r\n\r\nThis is a body"),
        http::Request::MalformedRequestException);

    ASSERT_THROW(http::Request("GET / HTTP/1.1\r\nHost:\r\nContent-Length: "
                               "15\r\n\r\nThis is a body"),
                 http::Request::MalformedRequestException);
    ASSERT_THROW(
        http::Request("GET / HTTP/1.1\r\nHost:x\r\nContent-Length: 15\r\n\r\n"),
        http::Request::MalformedRequestException);
}

TEST(testRequest, testComplexRequestString) {
    http::Request req(
        "GET /cgi-bin/myscript.cgi/path/to/script?param1=value1&param2=value2 "
        "HTTP/1.1\r\nHost: example.com\r\nUser-Agent: Mozilla/5.0 (Windows NT "
        "10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) "
        "Chrome/96.0.4664.45 Safari/537.36\r\nAccept: "
        "text/html,application/xhtml+xml,application/xml;q=0.9,image/"
        "avif,image/webp,image/apng,*/*;q=0.8,application/"
        "signed-exchange;v=b3;q=0.9\r\nAccept-Encoding: gzip, deflate, "
        "br\r\nAccept-Language: en-US,en;q=0.9\r\nConnection: "
        "keep-alive\r\n\r\n");

    EXPECT_EQ(req.getMethod(), "GET");
    EXPECT_EQ(req.getVersion(), "HTTP/1.1");
    EXPECT_EQ(req.getHeader("Host"), "example.com");
    EXPECT_EQ(req.getHeader("User-Agent"),
              "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 "
              "(KHTML, like Gecko) Chrome/96.0.4664.45 Safari/537.36");
    EXPECT_EQ(req.getHeader("Accept"),
              "text/html,application/xhtml+xml,application/xml;q=0.9,image/"
              "avif,image/"
              "webp,image/apng,*/*;q=0.8,application/"
              "signed-exchange;v=b3;q=0.9");
    EXPECT_EQ(req.getHeader("Accept-Encoding"), "gzip, deflate, br");
    EXPECT_EQ(req.getHeader("Accept-Language"), "en-US,en;q=0.9");
    EXPECT_EQ(req.getHeader("Connection"), "keep-alive");
    EXPECT_EQ(req.getBody(), "");
}

TEST(testRequest, testUriGetters) {
    std::string req = "GET /cgi-bin/python/me.py HTTP/1.1\r\nHost: x\r\n\r\n ";
    http::Request request(req);
    EXPECT_TRUE(request.getScheme().empty());
    EXPECT_TRUE(request.getHost().empty()) << request.getHost();
    EXPECT_TRUE(request.getPort().empty());
    EXPECT_EQ(request.getPath(), "/cgi-bin/python/me.py");
    EXPECT_TRUE(request.getQuery().empty());
}

TEST(testRequest, testMalformedRequestError) {
    std::string req =
        "GET "
        "/cgi/myscript.py/path/to/script?param1=value1&param2=value2 "
        "HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
        "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.45 "
        "Safari/537.36\r\n"
        "Accept: "
        "text/html,application/xhtml+xml,application/xml;q=0.9,image/"
        "avif,image/webp,image/apng,*/*;q=0.8,application/"
        "signed-exchange;v=b3;q=0.9\r\n"
        "Accept-Encoding: gzip, deflate, br\r\n"
        "Accept-Language: en-US,en;q=0.9\r\n"
        "Connection: keep-alive\r\n\r\n";
    ASSERT_NO_THROW(http::Request request(req));
}

TEST(testRequest, testRoutes) {
    std::string req =
        "GET "
        "/python/me.py/path/to/script?param1=value1&param2=value2 "
        "HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
        "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.45 "
        "Safari/537.36\r\n"
        "Accept: "
        "text/html,application/xhtml+xml,application/xml;q=0.9,image/"
        "avif,image/webp,image/apng,*/*;q=0.8,application/"
        "signed-exchange;v=b3;q=0.9\r\n"
        "Accept-Encoding: gzip, deflate, br\r\n"
        "Accept-Language: en-US,en;q=0.9\r\n"
        "Connection: keep-alive\r\n\r\n";
    smt::shared_ptr<http::Request> request;
    ASSERT_NO_THROW(request = smt::make_shared(new http::Request(req)));
    smt::shared_ptr<http::Route> route(
        new http::Route("/python/", "./websites/python/"));
    ASSERT_NO_THROW(request->setRoute(route));
    ASSERT_EQ(request->routeRequest(),
              "./websites/python/me.py/path/to/script");
}
