#include "config/Options.hpp"
#include "http/methods.hpp"
#include "http/Request.hpp"
#include "http/Response.hpp"
#include "utils/Route.hpp"

#include <gtest/gtest.h>

TEST(testConvert, testConvertMethod) {
    ASSERT_EQ(http::convertMethod("GET"), http::MethodType::GET);
    ASSERT_EQ(http::convertMethod("HEAD"), http::MethodType::HEAD);
    ASSERT_EQ(http::convertMethod("POST"), http::MethodType::POST);
    ASSERT_EQ(http::convertMethod("PUT"), http::MethodType::PUT);
    ASSERT_EQ(http::convertMethod("DELETE"), http::MethodType::DELETE);
    ASSERT_EQ(http::convertMethod("CONNECT"), http::MethodType::CONNECT);
    ASSERT_EQ(http::convertMethod("OPTIONS"), http::MethodType::OPTIONS);
    ASSERT_EQ(http::convertMethod("TRACE"), http::MethodType::TRACE);
    ASSERT_EQ(http::convertMethod("PATCH"), http::MethodType::PATCH);
    ASSERT_EQ(http::convertMethod("INVALID"), http::MethodType::INVALID);
    ASSERT_EQ(http::convertMethod("other"), http::MethodType::UNKNOWN_METHOD);
}

TEST(testConvert, testConvertVersion) {
    ASSERT_EQ(http::convertVersion("HTTP/1.1"), http::Version::HTTP_1_1);
    ASSERT_EQ(http::convertVersion("HTTP/1.0"), http::Version::UNKNOWN_VERSION);
    ASSERT_EQ(http::convertVersion("HTTP/2.0"), http::Version::UNKNOWN_VERSION);
}

class testGetMethod : public ::testing::Test {
    protected:

        void SetUp(void) {
            m_opts = smt::make_shared(new config::Opts);
            m_opts->m_target = "/";
            m_opts->m_root = "/tmp/";
            m_opts->m_index = "";
            m_opts->m_autoindex = false;
            m_route = smt::make_shared(
                new http::Route(m_opts->m_target, m_opts->m_root));
        }

        smt::shared_ptr<config::Opts> m_opts;
        smt::shared_ptr<http::Route>  m_route;
};

TEST_F(testGetMethod, testFileNotFound) {
    std::string reqStr = "GET /fileDoesNotExist HTTP/1.1\r\n"
                         "Host: localhost:8080\r\n\r\n";

    smt::shared_ptr<http::Request> req(new http::Request(reqStr));
    req->setRoute(m_route);

    smt::shared_ptr<http::Response> res = http::methods::GET(req, m_opts);

    ASSERT_EQ(res->getCode(), 404);
    ASSERT_FALSE(res->getBody().empty());
}

TEST_F(testGetMethod, testDirectoryNotFound) {
    std::string reqStr = "GET /dirDoesNotExist/ HTTP/1.1\r\n"
                         "Host: localhost:8080\r\n\r\n";

    smt::shared_ptr<http::Request> req(new http::Request(reqStr));
    req->setRoute(m_route);

    smt::shared_ptr<http::Response> res = http::methods::GET(req, m_opts);

    ASSERT_EQ(res->getCode(), 404);
    ASSERT_FALSE(res->getBody().empty());
}

TEST_F(testGetMethod, testFileFound) {
    system("touch /tmp/testFile.html");
    system("echo -n \"<h1>test</h1>\" > /tmp/testFile.html");
    std::string reqStr = "GET /testFile.html HTTP/1.1\r\n"
                         "Host: localhost:8080\r\n\r\n";

    smt::shared_ptr<http::Request> req(new http::Request(reqStr));
    req->setRoute(m_route);

    smt::shared_ptr<http::Response> res = http::methods::GET(req, m_opts);

    EXPECT_EQ(res->getCode(), 200);
    EXPECT_EQ(res->getHeader("Content-Type"), "text/html");
    EXPECT_EQ(res->getBody(), "<h1>test</h1>");
    system("rm -rf /tmp/testFile.html");
}

TEST_F(testGetMethod, testDirectoryFound) {
    system("mkdir -p /tmp/testDirectory");
    std::string reqStr = "GET /testDirectory/ HTTP/1.1\r\n"
                         "Host: localhost:8080\r\n\r\n";

    smt::shared_ptr<http::Request> req(new http::Request(reqStr));
    req->setRoute(m_route);

    smt::shared_ptr<http::Response> res = http::methods::GET(req, m_opts);

    EXPECT_EQ(res->getCode(), 403);
    EXPECT_FALSE(res->getBody().empty());
    system("rm -rf /tmp/testDirectory");
}

TEST_F(testGetMethod, testIndex) {
    // creating index file
    system("touch /tmp/indextest.html");
    system("echo -n \"<h1>Test Index</h1>\" > /tmp/indextest.html");
    m_opts->m_index = "/tmp/indextest.html";

    system("mkdir -p /tmp/testDirectory");
    std::string reqStr = "GET /testDirectory/ HTTP/1.1\r\n"
                         "Host: localhost:8080\r\n\r\n";

    smt::shared_ptr<http::Request> req(new http::Request(reqStr));
    req->setRoute(m_route);
    smt::shared_ptr<http::Response> res = http::methods::GET(req, m_opts);

    EXPECT_EQ(res->getCode(), 200);
    EXPECT_EQ(res->getBody(), "<h1>Test Index</h1>");
    EXPECT_EQ(res->getHeader("Content-Type"), "text/html");
    EXPECT_EQ(res->getHeader("Content-Length"), "19");

    system("rm -rf /tmp/testDirectory");
    system("rm -f /tmp/indextest.html");
}

TEST_F(testGetMethod, testIndexNotFound) {

    m_opts->m_index = "/tmp/indextest.html";

    system("mkdir -p /tmp/testDirectory");
    std::string reqStr = "GET /testDirectory/ HTTP/1.1\r\n"
                         "Host: localhost:8080\r\n\r\n";

    smt::shared_ptr<http::Request> req(new http::Request(reqStr));
    req->setRoute(m_route);
    smt::shared_ptr<http::Response> res = http::methods::GET(req, m_opts);

    EXPECT_EQ(res->getCode(), 404);
    EXPECT_FALSE(res->getBody().empty());

    system("rm -rf /tmp/testDirectory");
}

TEST_F(testGetMethod, testAutoIndexAndIndex) {
    // creating index file
    system("touch /tmp/indextest.html");
    system("echo -n \"<h1>Test Index</h1>\" > /tmp/indextest.html");
    m_opts->m_index = "/tmp/indextest.html";
    m_opts->m_autoindex = true;

    system("mkdir -p /tmp/testDirectory");
    std::string reqStr = "GET /testDirectory/ HTTP/1.1\r\n"
                         "Host: localhost:8080\r\n\r\n";

    smt::shared_ptr<http::Request> req(new http::Request(reqStr));
    req->setRoute(m_route);
    smt::shared_ptr<http::Response> res = http::methods::GET(req, m_opts);

    EXPECT_EQ(res->getCode(), 200);
    EXPECT_EQ(res->getBody(), "<h1>Test Index</h1>");
    EXPECT_EQ(res->getHeader("Content-Type"), "text/html");
    EXPECT_EQ(res->getHeader("Content-Length"), "19");

    system("rm -rf /tmp/testDirectory");
    system("rm -f /tmp/indextest.html");
}

TEST_F(testGetMethod, testAutoIndex) {

    m_opts->m_autoindex = true;

    system("mkdir -p /tmp/testDirectory");
    std::string reqStr = "GET /testDirectory HTTP/1.1\r\n"
                         "Host: localhost:8080\r\n\r\n";

    smt::shared_ptr<http::Request> req(new http::Request(reqStr));
    req->setRoute(m_route);
    smt::shared_ptr<http::Response> res = http::methods::GET(req, m_opts);

    EXPECT_EQ(res->getCode(), 200);
    EXPECT_FALSE(res->getBody().empty());
    EXPECT_EQ(res->getHeader("Content-Type"), "text/html");

    system("rm -rf /tmp/testDirectory");
}

class testPostMethod : public ::testing::Test {
    protected:

        void SetUp(void) {
            m_opts = smt::make_shared(new config::Opts);
            m_opts->m_target = "/";
            m_opts->m_root = "/tmp/";
            m_opts->m_index = "";
            m_opts->m_autoindex = false;
            m_route = smt::make_shared(
                new http::Route(m_opts->m_target, m_opts->m_root));
        }

        smt::shared_ptr<config::Opts> m_opts;
        smt::shared_ptr<http::Route>  m_route;
};

TEST_F(testPostMethod, testCrawlerFile) {
    std::string reqStr = "POST /../testFile HTTP/1.1\r\n"
                         "Host: localhost:8080\r\n"
                         "Content-Length: 12\r\n"
                         "Content-Type: text/plain\r\n\r\n"
                         "Hello World!";

    smt::shared_ptr<http::Request> req(new http::Request(reqStr));
    req->setRoute(m_route);
    smt::shared_ptr<http::Response> res = http::methods::POST(req, m_opts);
    EXPECT_EQ(res->getCode(), 404);
    EXPECT_FALSE(res->getBody().empty());
}

TEST_F(testPostMethod, testFileDoesNotExist) {
    std::string reqStr = "POST /testFile HTTP/1.1\r\n"
                         "Host: localhost:8080\r\n"
                         "Content-Length: 12\r\n"
                         "Content-Type: text/plain\r\n\r\n"
                         "Hello World!";

    smt::shared_ptr<http::Request> req(new http::Request(reqStr));
    req->setRoute(m_route);
    smt::shared_ptr<http::Response> res = http::methods::POST(req, m_opts);
    EXPECT_EQ(res->getCode(), 201);
    EXPECT_TRUE(res->getBody().empty());

    // getting file content
    reqStr = "GET /testFile HTTP/1.1\r\n"
             "Host: localhost:8080\r\n\r\n";
    req = smt::make_shared(new http::Request(reqStr));
    req->setRoute(m_route);

    res = http::methods::GET(req, m_opts);
    EXPECT_EQ(res->getCode(), 200);
    EXPECT_EQ(res->getBody(), "Hello World!");

    system("rm -f /tmp/testFile");
}

TEST_F(testPostMethod, testFileExists) {
    std::string reqStr = "POST /testFile HTTP/1.1\r\n"
                         "Host: localhost:8080\r\n"
                         "Content-Length: 12\r\n"
                         "Content-Type: text/plain\r\n\r\n"
                         "Hello World!";

    smt::shared_ptr<http::Request> req(new http::Request(reqStr));
    req->setRoute(m_route);
    smt::shared_ptr<http::Response> res = http::methods::POST(req, m_opts);
    EXPECT_EQ(res->getCode(), 201);
    EXPECT_TRUE(res->getBody().empty());
    res = http::methods::POST(req, m_opts);
    EXPECT_EQ(res->getCode(), 201);
    EXPECT_TRUE(res->getBody().empty());

    // getting file content
    reqStr = "GET /testFile HTTP/1.1\r\n"
             "Host: localhost:8080\r\n\r\n";
    req = smt::make_shared(new http::Request(reqStr));
    req->setRoute(m_route);

    res = http::methods::GET(req, m_opts);
    EXPECT_EQ(res->getCode(), 200);
    EXPECT_EQ(res->getBody(), "Hello World!Hello World!");

    system("rm -f /tmp/testFile");
}

class testDeleteMethod : public ::testing::Test {
    protected:

        void SetUp(void) {
            m_opts = smt::make_shared(new config::Opts);
            m_opts->m_target = "/";
            m_opts->m_root = "/tmp/";
            m_opts->m_index = "";
            m_opts->m_autoindex = false;
            m_route = smt::make_shared(
                new http::Route(m_opts->m_target, m_opts->m_root));
        }

        smt::shared_ptr<config::Opts> m_opts;
        smt::shared_ptr<http::Route>  m_route;
};

TEST_F(testDeleteMethod, testIsDirectory) {
    system("mkdir -p /tmp/testDirectory");
    std::string reqStr = "DELETE /testDirectory HTTP/1.1\r\n"
                         "Host: localhost:8080\r\n\r\n";

    smt::shared_ptr<http::Request> req(new http::Request(reqStr));
    req->setRoute(m_route);
    smt::shared_ptr<http::Response> res = http::methods::DELETE(req, m_opts);
    EXPECT_EQ(res->getCode(), 403);
    EXPECT_FALSE(res->getBody().empty());
    system("rm -rf /tmp/testDirectory");
}

TEST_F(testDeleteMethod, testFileDoesNotExist) {

    std::string reqStr = "DELETE /testFile HTTP/1.1\r\n"
                         "Host: localhost:8080\r\n\r\n";

    smt::shared_ptr<http::Request> req(new http::Request(reqStr));
    req->setRoute(m_route);
    smt::shared_ptr<http::Response> res = http::methods::DELETE(req, m_opts);
    EXPECT_EQ(res->getCode(), 404);
    EXPECT_FALSE(res->getBody().empty());
}

TEST_F(testDeleteMethod, testDeleteFile) {

    system("touch /tmp/testFile");
    std::string reqStr = "DELETE /testFile HTTP/1.1\r\n"
                         "Host: localhost:8080\r\n\r\n";

    smt::shared_ptr<http::Request> req(new http::Request(reqStr));
    req->setRoute(m_route);
    smt::shared_ptr<http::Response> res = http::methods::DELETE(req, m_opts);
    EXPECT_EQ(res->getCode(), 204);
    EXPECT_TRUE(res->getBody().empty());
    system("rm -f /tmp/testFile");
}
