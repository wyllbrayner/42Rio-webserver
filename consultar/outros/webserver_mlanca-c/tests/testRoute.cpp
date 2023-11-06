#include "http/Request.hpp"
#include "utils/Route.hpp"

#include <gtest/gtest.h>

TEST(testRoute, testRouteDirectory) {
    http::Route route("python", "./websites/cgi/python");
    ASSERT_EQ(route.getTarget(), "/python") << route.getTarget();
    ASSERT_EQ(route.getRoot(), "./websites/cgi/python/") << route.getRoot();
}

TEST(testRoute, testRouteFile) {
    http::Route route("index.py", "./websites/cgi/python");
    ASSERT_EQ(route.getTarget(), "/index.py") << route.getTarget();
    ASSERT_EQ(route.getRoot(), "./websites/cgi/python/") << route.getRoot();
    ASSERT_EQ(route.route("/index.py"), "./websites/cgi/python/");
    // ASSERT_EQ(route.reroute("./websites/cgo/python"), "index.py");
}

TEST(testRoute, testRoutingRequest) {
    http::Route route("python", "./websites/cgi/python");
    ASSERT_EQ(route.getTarget(), "/python") << route.getTarget();
    ASSERT_EQ(route.getRoot(), "./websites/cgi/python/") << route.getRoot();

    ASSERT_EQ(route.route("/python/index.py"),
              "./websites/cgi/python/index.py");
    // ASSERT_EQ(route.reroute("./websites/cgi/python/index.py"),
    //           "python/index.py");
}

TEST(testRoute, testRoutingRequestWithSlash) {
    http::Route route("python/", "./websites/cgi/python");
    ASSERT_EQ(route.getTarget(), "/python/") << route.getTarget();
    ASSERT_EQ(route.getRoot(), "./websites/cgi/python/") << route.getRoot();

    ASSERT_EQ(route.route("/python/index.py"),
              "./websites/cgi/python/index.py");
    // ASSERT_EQ(route.reroute("./websites/cgi/python/index.py"),
    //           "python/index.py");
}

TEST(testRoute, testRoutingRequestWithSlashAndSlashInTarget) {
    http::Route route("/python/", "./websites/cgi/python");
    ASSERT_EQ(route.getTarget(), "/python/") << route.getTarget();
    ASSERT_EQ(route.getRoot(), "./websites/cgi/python/") << route.getRoot();

    ASSERT_EQ(route.route("/python/index.py"),
              "./websites/cgi/python/index.py");
    // ASSERT_EQ(route.reroute("./websites/cgi/python/index.py"),
    //           "python/index.py");
}

TEST(testRoute, testRoutingBasic) {
    http::Route route("/", "./websites/cgi/python");
    ASSERT_EQ(route.getTarget(), "/") << route.getTarget();
    ASSERT_EQ(route.getRoot(), "./websites/cgi/python/") << route.getRoot();

    ASSERT_EQ(route.route("/"), "./websites/cgi/python/");
    // ASSERT_EQ(route.reroute("./websites/cgi/python/index.py"),
    //           "python/index.py");
}

TEST(testRoute, testRoutingForNoRouting) {
    http::Route route("/", "/");
    ASSERT_EQ(route.getTarget(), "/") << route.getTarget();
    ASSERT_EQ(route.getRoot(), "/") << route.getRoot();

    ASSERT_EQ(route.route("./websites/cgi-bin/index.py"),
              "./websites/cgi-bin/index.py");
    // ASSERT_EQ(route.reroute("./websites/cgi/python/index.py"),
    //           "python/index.py");
}