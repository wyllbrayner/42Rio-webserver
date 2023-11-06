#include "cgi/Argv.hpp"

#include "utils/ft_array.hpp"

namespace cgi {

Argv::Argv(void) {}

Argv::Argv(std::string const& scriptPath) { m_argv.push_back(scriptPath); }

Argv::Argv(Argv const& src) { *this = src; }

Argv::~Argv(void) {}

Argv& Argv::operator=(Argv const& rhs) {
    if (this != &rhs) { m_argv = rhs.m_argv; }
    return (*this);
}

Argv::operator char**(void) const { return (ft::array(m_argv)); }

} // namespace cgi
