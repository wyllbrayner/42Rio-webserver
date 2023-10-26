#include "cgi/Argv.hpp"
#include "utils/ft_array.hpp"

#include <gtest/gtest.h>

TEST(testArgv, testSimple) {
    cgi::Argv argv("scriptName");
    char**    c_argv = argv;
    EXPECT_STREQ(c_argv[0], "scriptName");
    EXPECT_TRUE(!c_argv[1]);
    ft::array::erase(c_argv);
}

TEST(testArgv, testCopy) {
    cgi::Argv argv("scriptName");
    cgi::Argv argv2(argv);
    char**    c_argv = argv2;
    EXPECT_STREQ(c_argv[0], "scriptName");
    EXPECT_TRUE(!c_argv[1]);
    ft::array::erase(c_argv);
}