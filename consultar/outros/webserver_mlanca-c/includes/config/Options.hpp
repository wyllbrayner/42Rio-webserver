#ifndef CONFIG_OPTIONS_HPP
#define CONFIG_OPTIONS_HPP

#include "config/LocationOpts.hpp"
#include "config/ServerOpts.hpp"
#include "http/Request.hpp"
#include "utils/ft_filesystem.hpp"
#include "utils/smt.hpp"

#include <set>
#include <sstream>
#include <vector>

namespace net {
class ServerSocket;
}

namespace config {
#define DEFAULT_BODY_SIZE 8192
class Opts;

class Options {
    public:

        ~Options(void);

        static smt::shared_ptr<Options> getInstance(
            std::vector<ServerOpts> const& options = std::vector<ServerOpts>());

        static smt::shared_ptr<Opts> getOptions(std::string const& port,
                                                std::string const& host,
                                                std::string const& path,
                                                std::string const& header = "");

        static std::set< std::pair<int, std::string> > getSocketOptions(void);

        struct NoSuchOptionsException : public std::exception {
                char const* what() const throw();
        };

    private:

        Options(void);
        Options(std::vector<ServerOpts> const& options);
        Options(Options const& src);

        Options& operator=(Options const& rhs);

        static int getCountOfDirs(std::vector<std::string> const& cmd,
                                  std::vector<std::string> const& target);

        std::vector<ServerOpts> m_options;
};

class Opts {

    public:

        ~Opts(void);
        Opts(void);
        Opts(ServerOpts const& serverOpts, LocationOpts const& locationOpts);
        Opts(ServerOpts const& serverOpts);

        bool operator==(Opts const& rhs) const;

        std::string             m_target;
        std::string             m_host;
        std::string             m_port;
        ft::directory           m_root;
        std::string             m_server_name;
        std::map<int, ft::file> m_error_pages;
        unsigned long           m_max_body_size;
        std::set<std::string>   m_allowed_methods;
        ft::file                m_index;
        bool                    m_autoindex;
        std::string             m_cgi_extension;
};

} // namespace config

#endif /* CONFIG_OPTIONS_HPP */