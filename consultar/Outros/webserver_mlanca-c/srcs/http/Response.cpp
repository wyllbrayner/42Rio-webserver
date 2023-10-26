#include "http/Response.hpp"

#include "http/methods.hpp"
#include "utils/ft_string.hpp"
#include "utils/Logger.hpp"

#include <sstream>
#include <vector>

namespace http {

#define SP   " "
#define CRLF "\r\n"

Response::Response(Response const& src) { *this = src; }

Response::Response(std::string const& respStr) {

    size_t endPos = respStr.find("\r\n\r\n");
    if (endPos == std::string::npos) {
        LOG_W("Malformed request: failed to find end of headers.");
        throw(MalformedResponseException());
    }

    std::istringstream iss(respStr);
    std::string        startLine;

    // getting first line
    getline(iss, startLine);

    // parsing and validating headers
    std::string line;
    while (getline(iss, line) && line != "\r") {

        if (line.find("\r") != line.size() - 1) {
            LOG_W("Malformed request: invalid header end.");
            throw(MalformedResponseException());
        }

        line = line.substr(0, line.size() - 1);
        size_t pos = line.find(": ");
        if (pos == std::string::npos || !pos || pos == line.size() - 2) {
            LOG_W("Malformed request: failed to parse headers.");
            throw(MalformedResponseException());
        }

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 2);
        m_headers[key] = value;
    }

    // parsing and validating start line
    if (startLine.empty()) {
        LOG_W("Malformed request: failed to find start line.");
        throw(MalformedResponseException());
    }
    if (startLine.find("\r") != startLine.size() - 1) {
        LOG_W("Malformed request: invalid start line end.");
        throw(MalformedResponseException());
    }
    startLine = startLine.substr(0, startLine.size() - 1);
    std::vector<std::string> subs = ft::string::split(startLine, " ");
    if (subs.size() < 3) {
        LOG_W("Malformed request: invalid start line number of arguments.");
        throw(MalformedResponseException());
    }
    if (convertVersion(subs[0]) == UNKNOWN_VERSION) {
        LOG_W("Malformed request: unknown version.");
        throw(MalformedResponseException());
    }
    m_version = subs[0];
    if (!ft::string::isnumeric(subs[1])) {
        LOG_W("Malformed request: invalid status code.");
        throw(MalformedResponseException());
    }
    m_code = ft::string::stoi(subs[1]);

    // parsing and validation body
    std::string body = respStr.substr(endPos + 4);
    if (m_headers.find("Content-Length") != m_headers.end()) {
        size_t len = ft::string::stoul(m_headers["Content-Length"]);
        if (body.size() != len) {
            LOG_W("Malformed request: invalid body size.");
            throw(MalformedResponseException());
        }
        m_body = body;
    }
    LOG_D(debug());
}

Response::Response(int code, std::map<std::string, std::string> headers,
                   std::string body, std::string version)
    : m_code(code), m_headers(headers), m_body(body), m_version(version) {

    if (m_headers.find("Content-Length") == m_headers.end()) {
        std::stringstream ss;
        ss << m_body.size();
        m_headers["Content-Length"] = ss.str();
    }
    if (m_headers.find("Content-Type") == m_headers.end()) {
        m_headers["Content-Type"] = "text/plain";
    }
    LOG_D(debug());
}

Response::~Response(void) {}

Response& Response::operator=(Response const& rhs) {
    if (this != &rhs) {
        m_code = rhs.m_code;
        m_headers = rhs.m_headers;
        m_body = rhs.m_body;
    }
    return (*this);
}

int Response::getCode(void) const { return (m_code); }

std::map<std::string, std::string> Response::getHeaders(void) const {
    return (m_headers);
}

std::string Response::getHeader(std::string const& key) const {
    return (m_headers.at(key));
}

std::string Response::getBody(void) const { return (m_body); }

std::string Response::getVersion(void) const { return (m_version); }

std::string Response::getReason(void) const { return (m_code); }

std::string const Response::toString(void) const {

    // converting m_code to string
    std::stringstream ss;
    ss << int(m_code);
    std::string code = ss.str();

    // converting m_header to string
    std::string headers;
    for (std::map<std::string, std::string>::const_iterator it =
             m_headers.begin();
         it != m_headers.end(); it++)
        headers += (*it).first + ":" + SP + (*it).second + CRLF;

    return (m_version + SP + code + SP + std::string(m_code) + CRLF + headers +
            CRLF + m_body);
}

std::string const Response::debug(void) const {
    // converting m_code to string
    std::stringstream ss;
    ss << int(m_code);
    std::string code = ss.str();

    return (m_version + SP + code + SP + std::string(m_code));
}

char const* Response::MalformedResponseException::what(void) const throw() {
    return ("Response: malformed response.");
}

} // namespace http
