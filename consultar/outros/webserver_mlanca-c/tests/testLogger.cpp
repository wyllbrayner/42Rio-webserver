#include "utils/Logger.hpp"
#include "utils/smt.hpp"

#include <gtest/gtest.h>
#include <sstream>

TEST(testLogger, testConstructor) {
    Logger& logger = Logger::getInstance();

    EXPECT_EQ(logger.getLevel(), LogLevel::Debug);
}

TEST(testLogger, testSetLevel) {
    Logger& logger = Logger::getInstance();

    ASSERT_THROW(logger.setFile("path/does/not/exist.log"),
                 ft::InvalidFileException);
    ASSERT_NO_THROW(logger.setFile("test.log"));
    system("rm test.log");
}

TEST(testLogger, testLog) {
    ASSERT_NO_THROW(
        Logger::getInstance().log(LogLevel::Debug, __FILE__, __LINE__));
}

TEST(testLogger, testLogLevels) {
    Logger& logger = Logger::getInstance();
    EXPECT_EQ(logger.getLevel(), LogLevel::Debug);

    logger.setLevel(LogLevel::Info);
    EXPECT_EQ(logger.getLevel(), LogLevel::Info);

    logger.setLevel(LogLevel::Warning);
    EXPECT_EQ(logger.getLevel(), LogLevel::Warning);

    logger.setLevel(LogLevel::Error);
    EXPECT_EQ(logger.getLevel(), LogLevel::Error);

    logger.setLevel(LogLevel::Fatal);
    EXPECT_EQ(logger.getLevel(), LogLevel::Fatal);

    logger.setLevel(LogLevel::Debug);
    EXPECT_EQ(logger.getLevel(), LogLevel::Debug);
}

TEST(testLogger, testMacros) {
    LOG_D("This is a debug message");
    LOG_I("This is an info message");
    LOG_W("This is a warning message");
    LOG_E("This is an error message");
    LOG_F("This is a fatal message");

    FLOG_D("This is a debug message");
    FLOG_I("This is an info message");
    FLOG_W("This is a warning message");
    FLOG_E("This is an error message");
    FLOG_F("This is a fatal message");

    LOG_LVL(LogLevel::Fatal);
    LOG_D("This message should not appear");
    LOG_LVL(LogLevel::Debug);
}

TEST(testFormatter, testDebug) {
    std::string str = Logger::Formatter::debug("file", 1);
    EXPECT_NE(str.find("DEBUG"), std::string::npos);
    EXPECT_NE(str.find("file"), std::string::npos);
    EXPECT_NE(str.find("1"), std::string::npos);
}

TEST(testFormatter, testInfo) {
    std::string str = Logger::Formatter::info("file", 1);
    EXPECT_NE(str.find("INFO"), std::string::npos);
    EXPECT_NE(str.find("file"), std::string::npos);
    EXPECT_NE(str.find("1"), std::string::npos);
}

TEST(testFormatter, testWarning) {
    std::string str = Logger::Formatter::warning("file", 1);
    EXPECT_NE(str.find("WARNING"), std::string::npos);
    EXPECT_NE(str.find("file"), std::string::npos);
    EXPECT_NE(str.find("1"), std::string::npos);
}

TEST(testFormatter, testError) {
    std::string str = Logger::Formatter::error("file", 1);
    EXPECT_NE(str.find("ERROR"), std::string::npos);
    EXPECT_NE(str.find("file"), std::string::npos);
    EXPECT_NE(str.find("1"), std::string::npos);
}

TEST(testFormatter, testFatal) {
    std::string str = Logger::Formatter::fatal("file", 1);
    EXPECT_NE(str.find("FATAL"), std::string::npos);
    EXPECT_NE(str.find("file"), std::string::npos);
    EXPECT_NE(str.find("1"), std::string::npos);
}