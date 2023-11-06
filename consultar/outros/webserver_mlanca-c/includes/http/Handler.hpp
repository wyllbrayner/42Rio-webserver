#ifndef HTTP_HANDLER_HPP
#define HTTP_HANDLER_HPP

#include "utils/smt.hpp"

#include <string>

namespace config {
class Opts;
}

namespace net {
class ServerSocket;
}

namespace http {

class Request;
class Response;

smt::shared_ptr<Response>
    processRequest(int status, smt::shared_ptr<Request> const request,
                   smt::shared_ptr<config::Opts> const opts);

} // namespace http

#endif /* HTTP_HANDLER_HPP */