#include "config/LocationOpts.hpp"

namespace config {

LocationOpts::LocationOpts(std::string const& target)
    : m_target(target), m_max_body_size(0), m_autoindex(false) {}

LocationOpts::LocationOpts(LocationOpts const& src) { *this = src; }

LocationOpts::~LocationOpts(void) {}

LocationOpts& LocationOpts::operator=(LocationOpts const& src) {
    if (this != &src) {
        m_target = src.m_target;
        m_root = src.m_root;
        m_error_pages = src.m_error_pages;
        m_max_body_size = src.m_max_body_size;
        m_allowed_methods = src.m_allowed_methods;
        m_index = src.m_index;
        m_autoindex = src.m_autoindex;
        m_cgi_extension = src.m_cgi_extension;
    }
    return (*this);
}

bool LocationOpts::operator==(LocationOpts const& rhs) const {
    return (m_target == rhs.m_target && m_root == rhs.m_root &&
            m_error_pages == rhs.m_error_pages &&
            m_max_body_size == rhs.m_max_body_size &&
            m_allowed_methods == rhs.m_allowed_methods &&
            m_index == rhs.m_index && m_autoindex == rhs.m_autoindex &&
            m_cgi_extension == rhs.m_cgi_extension);
}

} // namespace config