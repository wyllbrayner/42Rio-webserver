#include "http/Request.hpp"

#include "http/methods.hpp"
#include "utils/Logger.hpp"

namespace http {

#define SP   " "
#define CRLF "\r\n"

Request::Request(std::string const& reqStr) : m_reqStr(reqStr) {
    std::vector<std::string> subs;

    size_t endPos = reqStr.find("\r\n\r\n");
    if (endPos == std::string::npos) {
        LOG_W("Malformed request: failed to find end of headers");
        throw(MalformedRequestException());
    }

    std::istringstream iss(reqStr);
    std::string        startLine;
    getline(iss, startLine); // getting first line

    // handle headers
    std::string line;
    while (getline(iss, line) && line != "\r") {
        if (line.find("\r") != line.size() - 1) {
            LOG_W("Malformed request: invalid header end");
            throw(MalformedRequestException());
        }
        line = line.substr(0, line.size() - 1);
        size_t pos = line.find(": ");
        if (pos == std::string::npos || !pos || pos == line.size() - 2) {
            LOG_W("Malformed request: failed to parse headers");
            throw(MalformedRequestException());
        }
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 2);
        m_headers[key] = value;
    }

    // handle start line
    if (startLine.empty()) {
        LOG_W("Malformed request: failed to find start line");
        throw(MalformedRequestException());
    }
    // getting startLine without \r
    if (startLine.find("\r") != startLine.size() - 1) {
        LOG_W("Malformed request: invalid start line end");
        throw(MalformedRequestException());
    }
    startLine = startLine.substr(0, startLine.size() - 1);

    subs = ft::string::split(startLine, " ");
    if (subs.size() != 3) {
        LOG_W("Malformed request: invalid start line number of arguments");
        throw(MalformedRequestException());
    }
    if (convertMethod(subs[0]) == UNKNOWN_METHOD) {
        LOG_W("Malformed request: unknown method");
        throw(MalformedRequestException());
    }
    m_method = subs[0];
    m_uri = smt::make_shared(new Uri(subs[1]));
    if (convertVersion(subs[2]) == UNKNOWN_VERSION) {
        LOG_W("Malformed request: unknown version");
        throw(MalformedRequestException());
    }
    m_version = subs[2];

    // handle body
    std::string body = reqStr.substr(endPos + 4);
    // convert Content-Length to int
    if (m_headers.find("Content-Length") != m_headers.end()) {
        size_t len = ft::string::stoul(m_headers["Content-Length"]);
        if (body.size() != len) {
            LOG_W("Malformed request: invalid body size");
            throw(MalformedRequestException());
        }
        m_body = body;
    }
    LOG_D(debug());
}

Request::Request(Request const& src) { *this = src; }

Request::~Request(void) {}

Request& Request::operator=(Request const& rhs) {
    if (this != &rhs) {
        m_method = rhs.m_method;
        m_version = rhs.m_version;
        m_headers = rhs.m_headers;
        m_body = rhs.m_body;
        m_uri = rhs.m_uri;
        m_reqStr = rhs.m_reqStr;
    }
    return (*this);
}

std::string const& Request::getMethod(void) const { return (m_method); }

std::string const& Request::getVersion(void) const { return (m_version); }

std::string const Request::getHeader(std::string const& key) const {
    if (m_headers.find(key) == m_headers.end()) { return (""); }
    return (m_headers.at(key));
}

std::string const& Request::getBody(void) const { return (m_body); }

std::string Request::getScheme(void) const { return (m_uri->getScheme()); }

std::string Request::getHost(void) const { return (m_uri->getHost()); }

std::string Request::getPort(void) const { return (m_uri->getPort()); }

ft::file Request::getPath(void) const { return (m_uri->getPath()); }

std::string Request::getQuery(void) const { return (m_uri->getQuery()); }

ft::directory const& Request::getRoot(void) const {
    return (m_route->getRoot());
}

void Request::setRoute(smt::shared_ptr<Route> route) { m_route = route; }

std::string Request::routeRequest(void) const {
    return (m_route->route(getPath()));
}

void Request::setPath(std::string path) { m_uri->setPath(path); }

std::string const& Request::toString(void) const { return (m_reqStr); }

std::string const Request::debug(void) const {
    return (m_method + SP + m_uri->toString() + SP + m_version);
}

char const* Request::MalformedRequestException::what(void) const throw() {
    return ("Request: malformed request.");
}

} // namespace http
