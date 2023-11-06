#include "http/Uri.hpp"

#include "utils/ft_string.hpp"
#include "utils/Logger.hpp"

namespace http {

Uri::Uri(std::string const& uri) : m_uri(uri) {

    std::string str = ft::string::trim(uri);

    size_t scheme_pos = std::string(str).find("://");
    if (scheme_pos != std::string::npos) {
        m_scheme = str.substr(0, scheme_pos);
        str = str.substr(scheme_pos + 3);

        size_t host_pos = str.find("/");
        if (host_pos != std::string::npos) {
            m_host = str.substr(0, host_pos);
            str = str.substr(host_pos);
        }
    }
    else if (str.find(":") < str.find("/")) {
        size_t port_pos = str.find(":");
        if (port_pos != std::string::npos) {
            m_host = str.substr(0, port_pos);
            str = str.substr(port_pos + 1);
        }
        size_t path_pos = str.find("/");
        if (path_pos != std::string::npos) {
            m_port = str.substr(0, path_pos);
            str = str.substr(path_pos);
        }
    }

    size_t path_pos = str.find("?");
    if (path_pos != std::string::npos) {

        m_path = str.substr(0, path_pos);
        m_query = str.substr(++path_pos);
    }
    else { m_path = str; }
}

Uri::~Uri(void) {}

std::string Uri::getScheme(void) const { return (m_scheme); }

std::string Uri::getHost(void) const { return (m_host); }

std::string Uri::getPort(void) const { return (m_port); }

ft::file Uri::getPath(void) const { return (m_path); }

std::string Uri::getQuery(void) const { return (m_query); }

void Uri::setPath(std::string path) { m_path = path; }

std::string const Uri::toString(void) const { return (m_uri); }

} // namespace http
