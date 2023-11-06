#ifndef CGI_ARGV_HPP
#define CGI_ARGV_HPP

#include <string>
#include <vector>

namespace cgi {

class Argv {

    public:

        Argv(void);
        Argv(std::string const& scriptPath);
        Argv(Argv const& src);

        ~Argv(void);

        Argv& operator=(Argv const& rhs);

        operator char**(void) const;

    private:

        std::vector<std::string> m_argv;
};

} // namespace cgi

#endif /* CGI_ARGV_HPP */