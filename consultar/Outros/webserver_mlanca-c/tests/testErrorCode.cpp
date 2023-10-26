#include "utils/ErrorCode.hpp"

#include <gtest/gtest.h>

TEST(testErrorCode, testConstructor) {
    http::ErrorCode code1(100);
    http::ErrorCode code2(code1);
    http::ErrorCode code3(200);
    code3 = code2;

    EXPECT_EQ(std::string(code1), "Continue");
    EXPECT_EQ(std::string(code2), "Continue");
    EXPECT_EQ(std::string(code3), "Continue");
}

TEST(testErrorCode, testStringConversion) {
    http::ErrorCode code1(200);
    std::string     reason = code1;
    EXPECT_EQ(reason, "OK");
}