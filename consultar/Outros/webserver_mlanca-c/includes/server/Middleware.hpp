#ifndef SERVER_MIDDLEWARE_HPP
#define SERVER_MIDDLEWARE_HPP

#include "utils/smt.hpp"

#include <string>

namespace config {
class Opts;
class Options;
} // namespace config

namespace http {
class Request;
} // namespace http

namespace net {

class ServerSocket;

} // namespace net

namespace webserv {

class Middleware {
    public:

        static void handleRecv(smt::shared_ptr<net::ServerSocket> sock, int fd);

        static smt::shared_ptr<config::Opts>
            getOptions(smt::shared_ptr<net::ServerSocket> sock,
                       smt::shared_ptr<http::Request>     request);

        static smt::shared_ptr<config::Opts>
            getOptions(smt::shared_ptr<net::ServerSocket> sock);
};

} // namespace webserv

#endif /* SERVER_MIDDLEWARE_HPP */