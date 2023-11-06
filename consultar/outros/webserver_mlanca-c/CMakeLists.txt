cmake_minimum_required(VERSION 2.8)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(PROJECT_NAME webserv)
project(${PROJECT_NAME})

option(test "Build gtests" OFF)

add_subdirectory(srcs)
if (test)
	add_subdirectory(tests)
	enable_testing()
    find_package(PkgConfig)
    pkg_check_modules(GTEST "gtest" REQUIRED)
    pkg_check_modules(GMOCK "gmock" REQUIRED)
endif()

# set(CMAKE_CXX_FLAGS "-g -Wall -Werror -Wextra -std=c++98")
set(CMAKE_CXX_FLAGS "-g -Wall -Werror -Wextra -g")

include_directories(${CMAKE_SOURCE_DIR}/includes)

add_executable(${PROJECT_NAME} srcs/main.cpp)
target_link_libraries(${PROJECT_NAME} 42::webserv_lib)