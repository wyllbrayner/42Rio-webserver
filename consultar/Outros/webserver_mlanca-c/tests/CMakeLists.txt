cmake_minimum_required(VERSION 2.8)

# include(FetchContent)
# FetchContent_Declare(
#   googletest
#   URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
# )
# # For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)

set(PROJECT_NAME webserv_utests)
project(${PROJECT_NAME})

enable_testing()
find_package(PkgConfig)

include_directories(${CMAKE_SOURCE_DIR}/../includes/)

add_executable(${PROJECT_NAME}
  test.cpp
  client/Client.cpp
  testArgv.cpp
  testArray.cpp
  testCgiHandler.cpp
  testDirectives.cpp
  testEnvp.cpp
  testErrorCode.cpp
  testFileSystem.cpp
  testHost.cpp
  testLine.cpp
  testLogger.cpp
  testMethods.cpp
  testMimeType.cpp
  testOptions.cpp
  testParser.cpp
  testRequest.cpp
  testRequestBuffer.cpp
  testResponse.cpp
  testRoute.cpp
  testServerSocket.cpp
  testSmt.cpp
  testSocketConnection.cpp
  testString.cpp
  testUri.cpp
)

add_test(webserv_gtest ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
  PRIVATE 42::webserv_lib
  PRIVATE gtest_main
)