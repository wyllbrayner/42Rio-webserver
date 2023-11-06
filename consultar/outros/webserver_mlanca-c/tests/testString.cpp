#include "utils/ft_string.hpp"

#include <gtest/gtest.h>

TEST(testString, testSplit) {
    ASSERT_EQ(ft::string::split("Hello World", " "),
              std::vector<std::string>({"Hello", "World"}));

    ASSERT_EQ(ft::string::split("type: something", ":"),
              std::vector<std::string>({"type", " something"}));

    ASSERT_EQ(ft::string::split("type: something", ": "),
              std::vector<std::string>({"type", "something"}));

    ASSERT_EQ(ft::string::split(":type::: something: else:::", ":"),
              std::vector<std::string>({"type", " something", " else"}));

    ASSERT_EQ(ft::string::split(":type::: something: else:::", ": "),
              std::vector<std::string>({"type", "something", "else"}));
}

TEST(testString, testTrim) {
    ASSERT_EQ(ft::string::trim("Hello World"), "Hello World");
    ASSERT_EQ(ft::string::trim(" Hello World"), "Hello World");
    ASSERT_EQ(ft::string::trim("Hello World "), "Hello World");
    ASSERT_EQ(ft::string::trim(" Hello World "), "Hello World");
    ASSERT_EQ(ft::string::trim(" \t\n\rHello World \t\n\r"), "Hello World");

    ASSERT_EQ(ft::string::trim(""), "");
    ASSERT_EQ(ft::string::trim("  \t\n"), "");
    ASSERT_EQ(ft::string::trim("  \t\ns"), "s");
    ASSERT_EQ(ft::string::trim("s  \t\n"), "s");
    ASSERT_EQ(ft::string::trim("verylongstringwithoutspaces"),
              "verylongstringwithoutspaces");
}

TEST(testString, testRemoveTrailingComma) {
    ASSERT_EQ(ft::string::removeTrailingComma("Hello World"), "Hello World");
    ASSERT_EQ(ft::string::removeTrailingComma("Hello World;"), "Hello World");

    ASSERT_EQ(ft::string::removeTrailingComma(""), "");
    ASSERT_EQ(ft::string::removeTrailingComma(";"), "");
}

TEST(testString, testIsNumeric) {
    ASSERT_TRUE(ft::string::isnumeric("123"));
    ASSERT_TRUE(ft::string::isnumeric("-123"));
    ASSERT_TRUE(ft::string::isnumeric("0"));
    ASSERT_TRUE(ft::string::isnumeric("-0"));

    ASSERT_FALSE(ft::string::isnumeric("Hello World"));
    ASSERT_FALSE(ft::string::isnumeric("-123a"));
    ASSERT_FALSE(ft::string::isnumeric("0a"));
    ASSERT_FALSE(ft::string::isnumeric("-a"));
    ASSERT_FALSE(ft::string::isnumeric("123 456"));
}

TEST(testString, testStoi) {
    ASSERT_EQ(ft::string::stoi("123"), 123);
    ASSERT_EQ(ft::string::stoi("-123"), -123);
    ASSERT_EQ(ft::string::stoi("0"), 0);
    ASSERT_EQ(ft::string::stoi("+123"), 123);
}