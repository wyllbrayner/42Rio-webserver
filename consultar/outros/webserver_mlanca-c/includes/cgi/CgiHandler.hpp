#ifndef CGI_HPP
#define CGI_HPP

#include "cgi/Argv.hpp"
#include "cgi/Envp.hpp"
#include "config/Options.hpp"
#include "http/Request.hpp"
#include "utils/smt.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

namespace cgi {

enum CgiType { Py, Php, Cgi, Unknown };

class CgiHandler {

    public:

        CgiHandler(void);
        CgiHandler(smt::shared_ptr<http::Request> const request);
        CgiHandler(CgiHandler const& src);

        ~CgiHandler(void);

        CgiHandler& operator=(CgiHandler const& rhs);

        Argv const& getArgv(void) const;
        Envp const& getEnvp(void) const;

        std::string run(void) const;

        static CgiType convertCgiExtension(std::string const& cgiExtension);

    private:

        std::string runAsChildProcess(void) const;
        std::string get(std::string key) const;

        ft::directory m_cgiDir;
        Argv          m_argv;
        Envp          m_envp;

        std::string m_body;
};

} // namespace cgi

#endif /* CGI_HPP */