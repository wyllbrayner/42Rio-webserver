#ifndef HTTP_URI_HPP
#define HTTP_URI_HPP

#include "utils/ft_filesystem.hpp"

#include <string>

namespace http {

class Uri {

    public:

        Uri(std::string const& uri);
        ~Uri(void);

        std::string getScheme(void) const;
        std::string getHost(void) const;
        std::string getPort(void) const;
        ft::file    getPath(void) const;
        std::string getQuery(void) const;
        std::string const toString(void) const;
        
        void setPath(std::string path);

    private:

        std::string m_uri;
        std::string m_scheme;
        std::string m_host;
        std::string m_port;
        ft::file    m_path;
        std::string m_query;
};

} // namespace http

#endif /* HTTP_URI_HPP */