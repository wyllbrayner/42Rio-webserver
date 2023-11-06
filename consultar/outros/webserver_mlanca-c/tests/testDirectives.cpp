#include "config/DirectiveTypeTraits.hpp"

#include <gtest/gtest.h>

TEST(testBlockDirective, testIsDirectiveOrRoute) {
    config::DirectiveTypeTraits<config::LineBlock> blockDirective({});
    EXPECT_FALSE(blockDirective.isBlockDirective());
    EXPECT_FALSE(blockDirective.isRouteDirective());
}

TEST(testBlockDirective, testIsValid) {
    EXPECT_FALSE(config::DirectiveTypeTraits<config::LineBlock>({"server", "}"})
                     .isValid());
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::LineBlock>({"server"}).isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::LineBlock>({"other", "{"})
                     .isValid());
    EXPECT_TRUE(config::DirectiveTypeTraits<config::LineBlock>({"server", "{"})
                    .isValid());
}

TEST(testRouteDirective, testIsDirectiveOrRoute) {
    config::DirectiveTypeTraits<config::LineRoute> blockDirective({});
    EXPECT_TRUE(blockDirective.isBlockDirective());
    EXPECT_FALSE(blockDirective.isRouteDirective());
}

TEST(testRouteDirective, testIsValid) {
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::LineRoute>({"location", "/", "}"})
            .isValid());
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::LineRoute>({"location"}).isValid());
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::LineRoute>({"location", "{"})
            .isValid());
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::LineRoute>({"other", "/", "{"})
            .isValid());
    EXPECT_TRUE(
        config::DirectiveTypeTraits<config::LineRoute>({"location", "/", "{"})
            .isValid());
    EXPECT_TRUE(config::DirectiveTypeTraits<config::LineRoute>(
                    {"location", "/python/", "{"})
                    .isValid());
}

TEST(testListenDirective, testIsDirectiveOrRoute) {
    config::DirectiveTypeTraits<config::LineListen> directive({});
    EXPECT_TRUE(directive.isBlockDirective());
    EXPECT_FALSE(directive.isRouteDirective());
}

TEST(testListenDirective, testIsValid) {

    EXPECT_TRUE(
        config::DirectiveTypeTraits<config::LineListen>({"listen", "443"})
            .isValid());
    EXPECT_TRUE(
        config::DirectiveTypeTraits<config::LineListen>({"listen", "localhost"})
            .isValid());
    EXPECT_TRUE(config::DirectiveTypeTraits<config::LineListen>(
                    {"listen", "443:localhost"})
                    .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::LineListen>(
                     {"other", "443:localhost"})
                     .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::LineListen>(
                     {"listen", "localhost:443:"})
                     .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::LineListen>(
                     {"listen", ":localhost"})
                     .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::LineListen>(
                     {"listen", "localhost", "443"})
                     .isValid());
}

TEST(testServerNameDirective, testIsDirectiveOrRoute) {
    config::DirectiveTypeTraits<config::LineServerName> directive({});
    EXPECT_TRUE(directive.isBlockDirective());
    EXPECT_FALSE(directive.isRouteDirective());
}

TEST(testServerNameDirective, testIsValid) {
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::LineServerName>({"server_name"})
            .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::LineServerName>(
                     {"other", "domain.com"})
                     .isValid());
    EXPECT_TRUE(config::DirectiveTypeTraits<config::LineServerName>(
                    {"server_name", "domain.com"})
                    .isValid());
}

TEST(testRootDirective, testIsDirectiveOrRoute) {
    config::DirectiveTypeTraits<config::LineRoot> directive({});
    EXPECT_TRUE(directive.isBlockDirective());
    EXPECT_TRUE(directive.isRouteDirective());
}

TEST(testRootDirective, testIsValid) {
    system("mkdir -p /tmp/testRootDirective");
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::LineRoot>({"root"}).isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::LineRoot>(
                     {"root", "invalid/directory"})
                     .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::LineRoot>(
                     {"other", "invalid/directory"})
                     .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::LineRoot>(
                     {"root", "/tmp/testRootDirective", "invalid"})
                     .isValid());
    EXPECT_TRUE(config::DirectiveTypeTraits<config::LineRoot>(
                    {"root", "/tmp/testRootDirective"})
                    .isValid());
    system("rm -rf /tmp/testRootDirective");
}

TEST(testErrorPageDirective, testIsDirectiveOrRoute) {
    config::DirectiveTypeTraits<config::LineErrorPage> directive({});
    EXPECT_TRUE(directive.isBlockDirective());
    EXPECT_TRUE(directive.isRouteDirective());
}

TEST(testErrorPageDirective, testIsValid) {
    system("touch /tmp/testErrorPageDirective");

    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::LineErrorPage>({"error_page"})
            .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::LineErrorPage>(
                     {"error_page", "400"})
                     .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::LineErrorPage>(
                     {"other", "400", "/tmp/testRootDirective"})
                     .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::LineErrorPage>(
                     {"error_page", "/tmp/testRootDirective", "400"})
                     .isValid());
    EXPECT_TRUE(config::DirectiveTypeTraits<config::LineErrorPage>(
                    {"error_page", "404", "/tmp/testErrorPageDirective"})
                    .isValid());
    EXPECT_TRUE(config::DirectiveTypeTraits<config::LineErrorPage>(
                    {"error_page", "404", "405", "/tmp/testErrorPageDirective"})
                    .isValid());
    system("rm -f /tmp/testErrorPageDirective");
}

TEST(testMaxBodySizeDirective, testIsDirectiveOrRoute) {
    config::DirectiveTypeTraits<config::LineMaxBodySize> directive({});
    EXPECT_TRUE(directive.isBlockDirective());
    EXPECT_TRUE(directive.isRouteDirective());
}

TEST(testMaxBodySizeDirective, testIsValid) {
    EXPECT_FALSE(config::DirectiveTypeTraits<config::LineMaxBodySize>(
                     {"client_max_body_size"})
                     .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::LineMaxBodySize>(
                     {"client_max_body_size", "400", "other"})
                     .isValid());
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::LineMaxBodySize>({"other", "400"})
            .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::LineMaxBodySize>(
                     {"client_max_body_size", "invalid"})
                     .isValid());
    EXPECT_TRUE(config::DirectiveTypeTraits<config::LineMaxBodySize>(
                    {"client_max_body_size", "8080808080"})
                    .isValid());
}

TEST(testAllowMethodDirective, testIsDirectiveOrRoute) {
    config::DirectiveTypeTraits<config::LineAllowMethods> directive({});
    EXPECT_TRUE(directive.isBlockDirective());
    EXPECT_TRUE(directive.isRouteDirective());
}

TEST(testAllowMethodDirective, testIsValid) {
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::LineAllowMethods>({"allow_methods"})
            .isValid());
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::LineAllowMethods>({"other", "GET"})
            .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::LineAllowMethods>(
                     {"allow_methods", "invalid"})
                     .isValid());
    EXPECT_TRUE(config::DirectiveTypeTraits<config::LineAllowMethods>(
                    {"allow_methods", "GET"})
                    .isValid());
    EXPECT_TRUE(config::DirectiveTypeTraits<config::LineAllowMethods>(
                    {"allow_methods", "GET", "POST", "DELETE"})
                    .isValid());
}

TEST(testIndexDirective, testIsDirectiveOrRoute) {
    config::DirectiveTypeTraits<config::LineIndex> directive({});
    EXPECT_FALSE(directive.isBlockDirective());
    EXPECT_TRUE(directive.isRouteDirective());
}

TEST(testIndexDirective, testIsValid) {
    system("touch /tmp/testIndexDirective");

    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::LineIndex>({"index"}).isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::LineIndex>(
                     {"other", "/tmp/testIndexDirective"})
                     .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::LineIndex>(
                     {"index", "/tmp/testIndexDirective", "other"})
                     .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::LineIndex>(
                     {"index", "invalid/file"})
                     .isValid());
    EXPECT_TRUE(config::DirectiveTypeTraits<config::LineIndex>(
                    {"index", "/tmp/testIndexDirective"})
                    .isValid());

    system("rm -f /tmp/testIndexDirective");
}

TEST(testAutoIndexDirective, testIsDirectiveOrRoute) {
    config::DirectiveTypeTraits<config::LineAutoIndex> directive({});
    EXPECT_FALSE(directive.isBlockDirective());
    EXPECT_TRUE(directive.isRouteDirective());
}

TEST(testAutoIndexDirective, testIsValid) {

    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::LineAutoIndex>({"autoindex"})
            .isValid());
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::LineAutoIndex>({"other", "on"})
            .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::LineAutoIndex>(
                     {"autoindex", "on", "other"})
                     .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::LineAutoIndex>(
                     {"autoindex", "invalid"})
                     .isValid());
    EXPECT_TRUE(
        config::DirectiveTypeTraits<config::LineAutoIndex>({"autoindex", "on"})
            .isValid());
    EXPECT_TRUE(
        config::DirectiveTypeTraits<config::LineAutoIndex>({"autoindex", "off"})
            .isValid());
}

TEST(testCgiExtensionDirective, testIsDirectiveOrRoute) {
    config::DirectiveTypeTraits<config::LineCgiExtension> directive({});
    EXPECT_FALSE(directive.isBlockDirective());
    EXPECT_TRUE(directive.isRouteDirective());
}

TEST(testCgiExtensionDirective, testIsValid) {
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::LineCgiExtension>({"fastcgi_pass"})
            .isValid());
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::LineCgiExtension>({"other", ".py"})
            .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::LineCgiExtension>(
                     {"fastcgi_pass", ".py", "other"})
                     .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::LineCgiExtension>(
                     {"fastcgi_pass", ".tmp"})
                     .isValid());
    EXPECT_TRUE(config::DirectiveTypeTraits<config::LineCgiExtension>(
                    {"fastcgi_pass", ".py"})
                    .isValid());
    EXPECT_TRUE(config::DirectiveTypeTraits<config::LineCgiExtension>(
                    {"fastcgi_pass", ".php"})
                    .isValid());
    EXPECT_TRUE(config::DirectiveTypeTraits<config::LineCgiExtension>(
                    {"fastcgi_pass", ".cgi"})
                    .isValid());
}