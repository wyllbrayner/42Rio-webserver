#ifndef CONFIG_SERVER_OPTIONS_HPP
#define CONFIG_SERVER_OPTIONS_HPP

#include "config/LocationOpts.hpp"
#include "utils/ft_filesystem.hpp"

#include <map>
#include <set>
#include <string>
#include <vector>

namespace config {

class ServerOpts {
    public:

        ServerOpts(void);
        ServerOpts(ServerOpts const& src);

        ~ServerOpts(void);

        ServerOpts& operator=(ServerOpts const& rhs);

        bool operator==(ServerOpts const& rhs) const;

        std::string             m_host;
        std::string             m_port;
        ft::directory           m_root;
        std::string             m_server_name;
        std::map<int, ft::file> m_error_pages;
        unsigned long           m_max_body_size;
        std::set<std::string>   m_allowed_methods;

        std::vector<LocationOpts> m_locations;
};

} // namespace config

#endif /* CONFIG_SERVER_OPTIONS_HPP */