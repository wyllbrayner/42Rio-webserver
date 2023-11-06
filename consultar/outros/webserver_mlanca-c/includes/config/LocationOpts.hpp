#ifndef CONFIG_LOCATION_OPTIONS_HPP
#define CONFIG_LOCATION_OPTIONS_HPP

#include "utils/ft_filesystem.hpp"

#include <map>
#include <set>
#include <string>

namespace config {

class LocationOpts {
    public:

        LocationOpts(void);
        LocationOpts(std::string const& target);
        LocationOpts(LocationOpts const& src);

        ~LocationOpts(void);

        LocationOpts& operator=(LocationOpts const& rhs);

        bool operator==(LocationOpts const& rhs) const;

        std::string             m_target;
        ft::directory           m_root;
        std::map<int, ft::file> m_error_pages;
        unsigned long           m_max_body_size;
        std::set<std::string>   m_allowed_methods;
        ft::file                m_index;
        bool                    m_autoindex;
        std::string             m_cgi_extension;
};

} // namespace config

#endif /* CONFIG_LOCATION_OPTIONS_HPP */