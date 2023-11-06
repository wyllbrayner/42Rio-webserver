#include "config/ServerOpts.hpp"

#include "utils/Host.hpp"

namespace config {

ServerOpts::ServerOpts(void) : m_max_body_size(0) {}

ServerOpts::ServerOpts(ServerOpts const& src) { *this = src; }

ServerOpts::~ServerOpts(void) {}

ServerOpts& ServerOpts::operator=(ServerOpts const& rhs) {
    if (this != &rhs) {
        m_host = rhs.m_host;
        m_port = rhs.m_port;
        m_root = rhs.m_root;
        m_server_name = rhs.m_server_name;
        m_error_pages = rhs.m_error_pages;
        m_max_body_size = rhs.m_max_body_size;
        m_allowed_methods = rhs.m_allowed_methods;
        m_locations = rhs.m_locations;
    }
    return (*this);
}

bool ServerOpts::operator==(ServerOpts const& rhs) const {
    return (m_host == rhs.m_host && m_port == rhs.m_port &&
            m_root == rhs.m_root && m_server_name == rhs.m_server_name &&
            m_error_pages == rhs.m_error_pages &&
            m_max_body_size == rhs.m_max_body_size &&
            m_allowed_methods == rhs.m_allowed_methods &&
            m_locations == rhs.m_locations);
}

} // namespace config
