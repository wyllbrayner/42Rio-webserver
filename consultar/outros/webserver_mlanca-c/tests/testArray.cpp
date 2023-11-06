#include "utils/ft_array.hpp"

#include <gtest/gtest.h>

TEST(testArray, testArray) {
    ft::array arr({"hello", "world"});
    char**    c_arr = arr;
    EXPECT_STREQ(c_arr[0], "hello");
    EXPECT_STREQ(c_arr[1], "world");
    EXPECT_TRUE(!c_arr[2]);
    ft::array::erase(c_arr);
}

TEST(testArray, testEmptyArray) {
    ft::array arr;
    char**    c_arr = arr;
    EXPECT_TRUE(!c_arr[0]);
    ft::array::erase(c_arr);
}