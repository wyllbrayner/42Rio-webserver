#include "utils/Logger.hpp"

#include <gtest/gtest.h>
#include <iostream>

int main(int argc, char* argv[]) {

    (void)argc;
    (void)argv;

    LOG_LVL(LogLevel::Debug);
    testing::InitGoogleTest(&argc, argv);

    return (RUN_ALL_TESTS());
}