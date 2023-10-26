#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include "http/Uri.hpp"
#include "utils/Route.hpp"
#include "utils/smt.hpp"

#include <map>
#include <string>

namespace http {

#define SP   " "
#define CRLF "\r\n"

class Request {
    public:

        Request(std::string const& reqStr);
        Request(Request const& src);

        ~Request(void);

        Request& operator=(Request const& rhs);

        std::string const& getMethod(void) const;
        std::string const& getVersion(void) const;
        std::string const  getHeader(std::string const& key) const;
        std::string const& getBody(void) const;

        std::string          getScheme(void) const;
        std::string          getHost(void) const;
        std::string          getPort(void) const;
        ft::file             getPath(void) const;
        std::string          getQuery(void) const;
        ft::directory const& getRoot(void) const;

        void setPath(std::string path);

        void        setRoute(smt::shared_ptr<Route> route);
        std::string routeRequest(void) const;

        std::string const& toString(void) const;
        std::string const  debug(void) const;

        struct MalformedRequestException : public std::exception {
                char const* what(void) const throw();
        };

    private:

        std::string                        m_method;
        std::string                        m_version;
        std::map<std::string, std::string> m_headers;
        std::string                        m_body;
        smt::shared_ptr<Uri>               m_uri;
        smt::shared_ptr<Route>             m_route;

        std::string m_reqStr;
};

} // namespace http

#endif /* HTTP_REQUEST_HPP */