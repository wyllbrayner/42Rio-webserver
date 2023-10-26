#include "http/RequestBuffer.hpp"

#include <gtest/gtest.h>

TEST(testRequestBuffer, testBasicRequest) {
    std::string req0 = "HTTP/1.1 404\r\nContent-Type=text/html\r\n\r\n";
    ASSERT_EQ(http::RequestBuffer::getNextRequest(1, req0), req0);
    ASSERT_TRUE(http::RequestBuffer::hasRequest(1));
    ASSERT_TRUE(http::RequestBuffer::getNextRequest(1).empty());
    ASSERT_FALSE(http::RequestBuffer::hasRequest(1));
    ASSERT_EQ(http::RequestBuffer::getNextRequest(2, req0), req0);
    ASSERT_EQ(http::RequestBuffer::getNextRequest(2, req0), req0);
    ASSERT_TRUE(http::RequestBuffer::getNextRequest(2).empty());
    ASSERT_FALSE(http::RequestBuffer::hasRequest(2));
}

TEST(testRequestBuffer, testBasicRequestWithBody) {
    std::string req0 =
        "HTTP/1.1 404\r\nContent-Length: 120\r\n\r\n"
        "..........................................................."
        ".............................................................";
    ASSERT_EQ(http::RequestBuffer::getNextRequest(1, req0), req0);
    ASSERT_EQ(http::RequestBuffer::getNextRequest(2, req0), req0);
    ASSERT_TRUE(http::RequestBuffer::getNextRequest(1).empty());
    ASSERT_TRUE(http::RequestBuffer::getNextRequest(2).empty());
}

TEST(testRequestBuffer, testManyRequests) {
    std::string req0 =
        "HTTP/1.1 404\r\nContent-Length: "
        "120\r\n\r\n..........................................................."
        ".............................................................";
    std::string req1 =
        "GET /hello.htm HTTP/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; "
        "MSIE5.01; Windows NT)\r\nHost: "
        "www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: "
        "gzip, deflate\r\nConnection: Keep-Alive\r\n\r\n";

    ASSERT_EQ(http::RequestBuffer::getNextRequest(1, req0 + req1), req0);
    ASSERT_EQ(http::RequestBuffer::getNextRequest(1), req1);
    ASSERT_TRUE(http::RequestBuffer::getNextRequest(1).empty());
}

TEST(testRequestBuffer, testIncompleteBody) {
    std::string req0 = "HTTP/1.1 404\r\nContent-Length: "
                       "120\r\n\r\n............................................"
                       "......................................................."
                       "....................";
    std::string req1 = ".";

    ASSERT_TRUE(http::RequestBuffer::getNextRequest(1, req0).empty());
    ASSERT_EQ(http::RequestBuffer::getNextRequest(2, req0 + req1), req0 + req1);
    ASSERT_TRUE(http::RequestBuffer::getNextRequest(2).empty());
    ASSERT_EQ(http::RequestBuffer::getNextRequest(1, req1), req0 + req1);
    ASSERT_TRUE(http::RequestBuffer::getNextRequest(1).empty());
}

TEST(testRequestBuffer, testExtreme) {
    std::string req0 =
        "HTTP/1.1 404\r\nContent-Length: 120\r\n\r\n"
        "..........................................................."
        ".............................................................";

    std::string req1 =
        "GET /hello.htm HTTP/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; "
        "MSIE5.01; Windows NT)\r\nHost: "
        "www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: "
        "gzip, deflate\r\nConnection: Keep-Alive\r\n\r\n";

    std::string req2 =
        "GET /hello.htm HTTP/1.0\r\nUser-Agent: Mozilla/4.0 (compatible; "
        "MSIE5.01; Windows NT)\r\nAccept-Language: en-us\r\nAccept-Encoding: "
        "gzip, deflate\nConnection: Keep-Alive\r\n\r\n";

    std::string req3 = "POST /cgi-bin/process.cgi HTTP/1.1\r\nUser-Agent: "
                       "Mozilla/4.0 (compatible; MSIE5.01; Windows "
                       "NT)\r\nHost: www.tutorialspoint.com\r\nContent-Type: "
                       "text/xml; charset=utf-8\r\n";

    std::string req4 =
        "Content-Length: 95\r\nAccept-Language: en-us\r\nAccept-Encoding: "
        "gzip, deflate\r\nConnection: Keep-Alive\r\n\r\n<?xml version=\"1.0\" "
        "encoding=\"utf-8\"?>\r\n<string "
        "xmlns=\"http://clearforest.com/\">string</string>";

    std::string req5 =
        "GET /hello.htm HTTP/1.0\r\nUser-Agent: Mozilla/4.0 (compatible; "
        "MSIE5.01; Windows NT)\r\nAccept-Language: en-us\r\nAccept-Encoding: "
        "gzip, deflate\nConnection: Keep-Alive\r\n\r\n";

    ASSERT_EQ(http::RequestBuffer::getNextRequest(1, req1 + req2 + req3), req1);
    ASSERT_EQ(http::RequestBuffer::getNextRequest(1), req2);
    ASSERT_TRUE(http::RequestBuffer::getNextRequest(1).empty());
    ASSERT_EQ(http::RequestBuffer::getNextRequest(2, req0), req0);
    ASSERT_TRUE(http::RequestBuffer::getNextRequest(2).empty());
    ASSERT_EQ(http::RequestBuffer::getNextRequest(1, req4 + req5), req3 + req4);
    ASSERT_EQ(http::RequestBuffer::getNextRequest(1), req5);
    ASSERT_TRUE(http::RequestBuffer::getNextRequest(1).empty());
}