#include "utils/Route.hpp"

namespace http {

Route::Route(void) {}

Route::Route(Route const& src) { *this = src; }

Route::Route(std::string const& target, ft::directory const& root)
    : m_target(target), m_root(root) {
    if (!m_target.empty() && m_target[0] != '/') { m_target = '/' + m_target; }
}

Route::~Route(void) {}

Route& Route::operator=(Route const& rhs) {
    if (this != &rhs) {
        m_target = rhs.m_target;
        m_root = rhs.m_root;
    }
    return (*this);
}

bool Route::operator==(Route const& rhs) const {
    return (m_target == rhs.m_target && m_root == rhs.m_root);
}

std::string const& Route::getTarget(void) const { return (m_target); }

ft::directory const& Route::getRoot(void) const { return (m_root); }

std::string Route::route(std::string const& path) const {
    if (m_root == m_target) { return (path); }
    std::string const& target = ft::directory(m_target);
    std::string        str = path;
    str.replace(0, target.size(), m_root);
    return (str);
}

// std::string Route::reroute(std::string const& path) const {
//     std::string const& root = m_root;
//     std::string        str = path;
//     if (root.size() == str.size()) { return (m_target); }
//     str.replace(0, root.size(), ft::directory(m_target));
//     return (str);
// }

} // namespace http
