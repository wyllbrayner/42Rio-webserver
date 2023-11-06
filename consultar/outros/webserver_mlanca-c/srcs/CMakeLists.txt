project(webserv_lib)

add_library(${PROJECT_NAME} STATIC
  cgi/Argv.cpp
  cgi/CgiHandler.cpp
  cgi/Envp.cpp
  config/DirectiveTypeTraits.cpp
  config/Line.cpp
  config/LocationOpts.cpp
  config/Options.cpp
  config/Parser.cpp
  config/ServerOpts.cpp
  http/Handler.cpp
  http/methods.cpp
  http/Request.cpp
  http/RequestBuffer.cpp
  http/Response.cpp
  http/Uri.cpp
  server/Middleware.cpp
  server/Server.cpp
  server/webserv.cpp
  sockets/ServerSocket.cpp
  sockets/SocketConnection.cpp
  utils/ErrorCode.cpp
  utils/ft_array.cpp
  utils/ft_exceptions.cpp
  utils/ft_filesystem.cpp
  utils/ft_string.cpp
  utils/Host.cpp
  utils/Logger.cpp
  utils/MimeType.cpp
  utils/Route.cpp
)

add_library(42::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/includes)