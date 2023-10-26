#ifndef HTTP_METHODS_HPP
#define HTTP_METHODS_HPP

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

typedef enum {
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH,
    INVALID,
    UNKNOWN_METHOD,
} MethodType;

typedef enum {
    HTTP_1_1,
    UNKNOWN_VERSION,
} Version;

MethodType convertMethod(std::string const& methodStr);
Version    convertVersion(std::string const& versionStr);

namespace methods {

smt::shared_ptr<http::Response>
    GET(smt::shared_ptr<http::Request> const request,
        smt::shared_ptr<config::Opts> const  opts);

smt::shared_ptr<http::Response>
    POST(smt::shared_ptr<http::Request> const request,
         smt::shared_ptr<config::Opts> const  opts);

smt::shared_ptr<http::Response>
    DELETE(smt::shared_ptr<http::Request> const request,
           smt::shared_ptr<config::Opts> const  opts);

smt::shared_ptr<http::Response>
    generateAutoIndexResponse(smt::shared_ptr<http::Request> const req);

smt::shared_ptr<http::Response>
    generateCgiResponse(smt::shared_ptr<http::Request> const request,
                        smt::shared_ptr<config::Opts> const  opts);

smt::shared_ptr<Response>
    generateErrorResponse(int code, smt::shared_ptr<config::Opts> const opts);

} // namespace methods

} // namespace http

#endif /* HTTP_METHODS_HPP */