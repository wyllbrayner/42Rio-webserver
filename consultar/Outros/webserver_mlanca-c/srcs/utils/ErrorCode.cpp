#include "utils/ErrorCode.hpp"

namespace http {

ErrorCode::ErrorCode(void) : m_code(0) {}

ErrorCode::ErrorCode(int code) : m_code(code) {}

ErrorCode::~ErrorCode(void) {}

ErrorCode::ErrorCode(ErrorCode const& src) { *this = src; }

ErrorCode::operator std::string(void) const {
    return (ErrorCode::getReason(m_code));
}

ErrorCode::operator int(void) const { return (m_code); }

ErrorCode& ErrorCode::operator=(unsigned int const& rhs) {
    m_code = rhs;
    return (*this);
}

ErrorCode& ErrorCode::operator=(ErrorCode const& str) {
    m_code = str.m_code;
    return (*this);
}

char const* ErrorCode::UnrecognizedErrorCode::what(void) const throw() {
    return ("status code: Unrecognized error code.");
}

std::string const ErrorCode::getReason(int code) {
    if (code == 100) { return ("Continue"); }
    if (code == 101) { return ("Switching Protocols"); }
    if (code == 102) { return ("Processing"); }
    if (code == 103) { return ("Early Hints"); }
    if (code == 200) { return ("OK"); }
    if (code == 201) { return ("Created"); }
    if (code == 202) { return ("Accepted"); }
    if (code == 203) { return ("Non Authoritative Info"); }
    if (code == 204) { return ("No Content"); }
    if (code == 205) { return ("Reset Content"); }
    if (code == 206) { return ("Partial Content"); }
    if (code == 207) { return ("Multi Status"); }
    if (code == 208) { return ("Already Reported"); }
    if (code == 226) { return ("IM Used"); }
    if (code == 300) { return ("Multiple Choices"); }
    if (code == 301) { return ("Moved Permanently"); }
    if (code == 302) { return ("Found"); }
    if (code == 303) { return ("See Other"); }
    if (code == 304) { return ("Not Modified"); }
    if (code == 305) { return ("Use Proxy"); }
    if (code == 307) { return ("Temporary Redirect"); }
    if (code == 308) { return ("Permanent Redirect"); }
    if (code == 400) { return ("Bad Request"); }
    if (code == 401) { return ("Unauthorized"); }
    if (code == 402) { return ("Payment Required"); }
    if (code == 403) { return ("Forbidden"); }
    if (code == 404) { return ("Not Found"); }
    if (code == 405) { return ("Method Not Allowed"); }
    if (code == 406) { return ("Not Acceptable"); }
    if (code == 407) { return ("Proxy Auth Required"); }
    if (code == 408) { return ("Request Timeout"); }
    if (code == 409) { return ("Conflict"); }
    if (code == 410) { return ("Gone"); }
    if (code == 411) { return ("Length Required"); }
    if (code == 412) { return ("Precondition Failed"); }
    if (code == 413) { return ("Request Entity Too Large"); }
    if (code == 414) { return ("Request URI Too Long"); }
    if (code == 415) { return ("Unsupported Media Type"); }
    if (code == 416) { return ("Requested Range Not Satisfiable"); }
    if (code == 417) { return ("Expectation Failed"); }
    if (code == 418) { return ("Teapot"); }
    if (code == 421) { return ("Misdirected Request"); }
    if (code == 422) { return ("Unprocessable Entity"); }
    if (code == 423) { return ("Locked"); }
    if (code == 424) { return ("Failed Dependency"); }
    if (code == 425) { return ("Too Early"); }
    if (code == 426) { return ("Upgrade Required"); }
    if (code == 428) { return ("Precondition Required"); }
    if (code == 429) { return ("Too Many Requests"); }
    if (code == 431) { return ("Request Header Fields Too Large"); }
    if (code == 451) { return ("Unavailable For Legal Reasons"); }
    if (code == 500) { return ("Internal Server Error"); }
    if (code == 501) { return ("Not Implemented"); }
    if (code == 502) { return ("Bad Gateway"); }
    if (code == 503) { return ("Service Unavailable"); }
    if (code == 504) { return ("GatewayTimeout"); }
    if (code == 505) { return ("HTTP Version Not Supported"); }
    if (code == 506) { return ("Variant Also Negotiates"); }
    if (code == 507) { return ("Insufficient Storage"); }
    if (code == 508) { return ("Loop Detected"); }
    if (code == 510) { return ("Not Extended"); }
    if (code == 511) { return ("Network Authentication Required"); }
    throw UnrecognizedErrorCode();
}

} // namespace http