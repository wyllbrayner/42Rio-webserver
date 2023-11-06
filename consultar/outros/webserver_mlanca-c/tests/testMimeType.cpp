#include "utils/MimeType.hpp"

#include <gtest/gtest.h>

TEST(testMimeType, testConstructor) {
    EXPECT_EQ(std::string(http::MimeType("somefile.txt")), "text/plain");
    EXPECT_EQ(std::string(http::MimeType("somefile.jpeg")), "image/jpeg");
    EXPECT_EQ(std::string(http::MimeType("somefile.html")), "text/html");
    EXPECT_EQ(std::string(http::MimeType("somefile.htm")), "text/html");
    EXPECT_EQ(std::string(http::MimeType("somefile.json")), "application/json");
}
