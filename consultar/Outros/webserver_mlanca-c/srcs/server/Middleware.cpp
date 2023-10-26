#include "server/Middleware.hpp"

#include "config/Options.hpp"
#include "http/Handler.hpp"
#include "http/Request.hpp"
#include "http/RequestBuffer.hpp"
#include "http/Response.hpp"
#include "sockets/ServerSocket.hpp"

namespace webserv {

void Middleware::handleRecv(smt::shared_ptr<net::ServerSocket> sock, int fd) {
    smt::shared_ptr<config::Opts> opts(new config::Opts());

    // receiving string from sockets recv
    std::string reqStr = sock->recv(fd);

    // parsing the request from reqStr
    int status = 0;
    reqStr = http::RequestBuffer::getNextRequest(fd, reqStr);
    while (!reqStr.empty()) {

        smt::shared_ptr<http::Request> request;
        try {
            // creating a http::Request based on string
            request = smt::make_shared(new http::Request(reqStr));
            // getting options from config file corresponding with request
            opts = getOptions(sock, request);
        }
        catch (http::Request::MalformedRequestException const&) {
            // checking if communication is valid HTTP/1.1
            LOG_E("Malformed request, socket will responde with a 400.");
            status = 400;
            opts = getOptions(sock);
        }
        catch (config::Options::NoSuchOptionsException const&) {
            // this error will never happen, but its handled anyways
            LOG_E("Failure in server, failed to find a config block for this "
                  "request.");
            status = 500;
            opts = getOptions(sock);
        }

        // Setting up route
        smt::shared_ptr<http::Route> route;
        route = smt::make_shared(new http::Route(opts->m_target, opts->m_root));
        request->setRoute(route);

        // handle http request
        smt::shared_ptr<http::Response> response =
            http::processRequest(status, request, opts);

        // send response as string
        sock->send(fd, response->toString());

        // getting next request form reqStr if there is one
        reqStr = http::RequestBuffer::getNextRequest(fd);
    }
}

smt::shared_ptr<config::Opts>
    Middleware::getOptions(smt::shared_ptr<net::ServerSocket> sock,
                           smt::shared_ptr<http::Request>     request) {

    // calling getOptions from options
    std::stringstream ss;
    ss << sock->getPort();
    return (config::Options::getOptions(ss.str(), sock->getHost(),
                                        request->getPath(),
                                        request->getHeader("Host")));
}

smt::shared_ptr<config::Opts>
    Middleware::getOptions(smt::shared_ptr<net::ServerSocket> sock) {

    // calling default getOpyions from options
    std::stringstream ss;
    ss << sock->getPort();
    return (config::Options::getOptions(ss.str(), sock->getHost(), "/"));
}

} // namespace webserv