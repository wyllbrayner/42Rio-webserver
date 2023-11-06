#ifndef HTTP_ROUTE_HPP
#define HTTP_ROUTE_HPP

#include "utils/ft_filesystem.hpp"
#include "utils/Logger.hpp"

#include <string>

namespace http {

class Route {
    public:

        Route(void);
        Route(Route const& src);
        Route(std::string const& target, ft::directory const& root);

        ~Route(void);

        Route& operator=(Route const& rhs);

        bool operator==(Route const& rhs) const;

        std::string const&   getTarget(void) const;
        ft::directory const& getRoot(void) const;

        std::string route(std::string const& path) const;

    private:

        std::string   m_target;
        ft::directory m_root;
};

} // namespace http

#endif /* HTTP_ROUTE_HPP */