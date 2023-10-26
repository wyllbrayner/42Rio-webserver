#ifndef RESOLVHOST_HPP
#define RESOLVHOST_HPP

#include "utils/Logger.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <string>
#include <vector>

namespace net {

class Host {
    public:

        Host(void);
        Host(char const* s);
        Host(std::string const& host);

        ~Host(void);

        Host& operator=(std::string const& str);
        Host& operator=(char const* str);

        operator std::string(void) const;

        bool operator==(Host const& rhs) const;

        std::vector<std::string> resolve(std::string const& port) const;

    private:

        std::string m_host;
};

} // namespace net

#endif /* RESOLVHOST_HPP */