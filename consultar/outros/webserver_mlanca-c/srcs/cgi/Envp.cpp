#include "cgi/Envp.hpp"

#include "http/Request.hpp"
#include "utils/ft_array.hpp"
#include "utils/ft_filesystem.hpp"
#include "utils/Logger.hpp"

namespace cgi {

Envp::Envp(void) {}

Envp::Envp(smt::shared_ptr<http::Request> request) {
    std::string filename = ft::file(request->routeRequest()).getFilename();
    std::string root = request->getRoot();
    m_envp.push_back("REQUEST_METHOD=" + request->getMethod());
    m_envp.push_back("PATH_INFO=" + filename);
    m_envp.push_back("DOCUMENT_ROOT=" + root);
    m_envp.push_back("QUERY_STRING=" + request->getQuery());
    m_envp.push_back("CONTENT_LENGTH=" + request->getHeader("Content-Length"));
    m_envp.push_back("CONTENT_TYPE=" + request->getHeader("Content-Type"));
}

Envp::Envp(Envp const& src) { *this = src; }

Envp::~Envp(void) {}

Envp& Envp::operator=(Envp const& rhs) {
    if (this != &rhs) { m_envp = rhs.m_envp; }
    return (*this);
}

std::string Envp::get(std::string key) const {
    for (size_t i = 0; i < m_envp.size(); i++) {
        if (m_envp[i].find(key) == 0) {
            return (m_envp[i].substr(key.size() + 1));
        }
    }
    return ("");
}

Envp::operator char**(void) const { return (ft::array(m_envp)); }

} // namespace cgi
