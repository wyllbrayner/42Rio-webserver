#ifndef CGI_ENVP_HPP
#define CGI_ENVP_HPP

#include "utils/smt.hpp"

#include <string>
#include <vector>

namespace http {
class Request;
}

namespace cgi {

class Envp {

    public:

        Envp(void);
        Envp(smt::shared_ptr<http::Request> request);
        Envp(Envp const& src);

        ~Envp(void);

        Envp& operator=(Envp const& rhs);

        std::string get(std::string key) const;

        operator char**(void) const;

    private:

        std::vector<std::string> m_envp;
};

} // namespace cgi

#endif /* CGI_ENVP_HPP */