#include "config/Parser.hpp"

#include <gtest/gtest.h>

class testParser : public ::testing::Test {
    protected:

        testParser(void) {
            system("touch test.conf");
            system("echo -n \"hello world\" > test.conf");
        }

        virtual ~testParser(void) { system("rm test.conf"); }
};

TEST_F(testParser, testFileOpen) {
    ASSERT_THROW(config::Parser(ft::file("file.conf")),
                 ft::InvalidFileException);

    ASSERT_NO_THROW(config::Parser("test.conf"));
}

TEST_F(testParser, testNextLine) {
    config::Parser parser("test.conf");

    ASSERT_TRUE(parser.nextLine());
    ASSERT_EQ(parser.getLine(), "hello world");
    ASSERT_EQ(parser.getPosition(), 1);
    ASSERT_FALSE(parser.nextLine());
    ASSERT_EQ(parser.getLine(), "hello world");
    ASSERT_EQ(parser.getPosition(), 1);
}

TEST_F(testParser, testError) {
    config::Parser parser("test.conf");

    parser.nextLine();
    ASSERT_THROW(parser.error(), config::Parser::InvalidSyntaxException);
}

TEST_F(testParser, testParseLine) {
    config::Parser parser("test.conf");
    ASSERT_NO_THROW(parser.parseLine<config::LineBlock>({"server", "{"}, 0));
    ASSERT_NO_THROW(parser.parseLine<config::LineListen>({"listen", "80"}, 1));
    ASSERT_NO_THROW(parser.parseLine<config::LineBlock>({"server", "{"}, 0));
    ASSERT_NO_THROW(parser.parseLine<config::LineBlock>({"server", "{"}, 0));
    ASSERT_NO_THROW(parser.parseLine<config::LineListen>({"listen", "80"}, 1));
    ASSERT_NO_THROW(
        parser.parseLine<config::LineRoute>({"location", "/", "{"}, 1));
    ASSERT_NO_THROW(
        parser.parseLine<config::LineRoute>({"location", "/", "{"}, 1));

    ASSERT_THROW(
        parser.parseLine<config::LineListen>({"listen", "80", "80"}, 1),
        config::Parser::InvalidSyntaxException);
}
