#include "cgi/Argv.hpp"
#include "cgi/CgiHandler.hpp"
#include "cgi/Envp.hpp"
#include "http/Response.hpp"
#include "utils/ft_array.hpp"
#include "utils/Logger.hpp"

#include <gtest/gtest.h>

TEST(testResponse, testConstructor) {
    http::Response response(404, std::map<std::string, std::string>());
    ASSERT_EQ(response.getCode(), 404);
    ASSERT_TRUE(response.getBody().empty());
    ASSERT_EQ(response.getHeaders().size(), 2);
    ASSERT_EQ(response.getHeader("Content-Length"), "0");
    ASSERT_EQ(response.getHeader("Content-Type"), "text/plain");
    ASSERT_EQ(response.getReason(), "Not Found");
}

TEST(testResponse, testResponseWithBody) {
    http::Response response(200, std::map<std::string, std::string>(),
                            "Hello World!");
    ASSERT_EQ(response.getCode(), 200);
    ASSERT_EQ(response.getBody(), "Hello World!");
    ASSERT_EQ(response.getHeaders().size(), 2);
    ASSERT_EQ(response.getHeader("Content-Length"), "12");
    ASSERT_EQ(response.getHeader("Content-Type"), "text/plain");
    ASSERT_EQ(response.getReason(), "OK");
}

TEST(testResponse, testResponseWithHeader) {
    std::map<std::string, std::string> headers;
    headers["Content-Length"] = "120";
    headers["Content-Type"] = "text/html";
    headers["Something"] = "else";
    http::Response response(404, headers, "Hello World!");
    ASSERT_EQ(response.getCode(), 404);
    ASSERT_EQ(response.getBody(), "Hello World!");
    ASSERT_EQ(response.getHeaders().size(), 3);
    ASSERT_EQ(response.getHeader("Content-Length"), "120");
    ASSERT_EQ(response.getHeader("Content-Type"), "text/html");
    ASSERT_EQ(response.getHeader("Something"), "else");
    ASSERT_EQ(response.getReason(), "Not Found");
}

TEST(testResponse, testResponseStringConversion) {
    std::map<std::string, std::string> header;
    ASSERT_EQ(http::Response(404, header).toString(),
              "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\nContent-Type: "
              "text/plain\r\n\r\n");

    ASSERT_EQ(http::Response(404, header, "Hello World!").toString(),
              "HTTP/1.1 404 Not Found\r\nContent-Length: 12\r\nContent-Type: "
              "text/plain\r\n\r\nHello World!");
}

TEST(testResponse, testResponseStringConstructor) {
    std::string respStr =
        "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\nContent-Type: "
        "text/plain\r\n\r\n";
    http::Response resp(respStr);
    ASSERT_EQ(resp.getCode(), 404);
    ASSERT_EQ(resp.getBody(), "");
    ASSERT_EQ(resp.getHeaders().size(), 2);
    ASSERT_EQ(resp.getHeader("Content-Length"), "0");
    ASSERT_EQ(resp.getHeader("Content-Type"), "text/plain");
    ASSERT_EQ(resp.getReason(), "Not Found");
    ASSERT_EQ(resp.toString(), respStr);
}

TEST(testResponse, testResponseStringConstructorWithBody) {
    std::string respStr =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 148\r\n"
        "\r\n"
        "<!DOCTYPE html> <html> <head>   <title>Hello "
        "Page</title> </head> <body>   <h1>Hello, World!</h1> "
        "  <p>Welcome to my hello page.</p> </body> </html>";

    http::Response resp(respStr);
    ASSERT_EQ(resp.getCode(), 200);
    ASSERT_EQ(resp.getBody(),
              "<!DOCTYPE html> <html> <head>   <title>Hello "
              "Page</title> </head> <body>   <h1>Hello, World!</h1>   "
              "<p>Welcome to my hello page.</p> </body> </html>");
    ASSERT_EQ(resp.getHeaders().size(), 2);
    ASSERT_EQ(resp.getHeader("Content-Length"), "148");
    ASSERT_EQ(resp.getHeader("Content-Type"), "text/html");
    ASSERT_EQ(resp.getReason(), "OK");
}