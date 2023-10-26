#ifndef CONFIG_DIRECTIVES_HPP
#define CONFIG_DIRECTIVES_HPP

#include "cgi/CgiHandler.hpp"
#include "config/Line.hpp"
#include "config/ServerOpts.hpp"
#include "http/methods.hpp"
#include "utils/ft_filesystem.hpp"
#include "utils/ft_string.hpp"
#include "utils/Host.hpp"
#include "utils/Logger.hpp"
#include "utils/smt.hpp"

#include <map>
#include <set>
#include <string>
#include <typeinfo>
#include <vector>

namespace config {

class DirectiveTypeTraitsBase {
    public:

        DirectiveTypeTraitsBase(std::string const& name);
        virtual ~DirectiveTypeTraitsBase(void);

        virtual std::string getName(void) const;

        virtual void set(std::vector<ServerOpts>&) const;
        virtual void set(ServerOpts&) const;
        virtual void set(LocationOpts&) const;

        virtual bool isValid(void) const;

        virtual bool isGlobalDirective(void) const;
        virtual bool isBlockDirective(void) const;
        virtual bool isRouteDirective(void) const;

        std::string m_name;
        bool        m_valid;
};

template<LineType Directive>
struct DirectiveTypeTraits;

template<>
struct DirectiveTypeTraits<LineBlock> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(std::vector<std::string> const& args)
            : DirectiveTypeTraitsBase(LINE_SERVER) {
            if (args.size() != 2) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive.");
                return;
            }
            if (args[1] != "{") {
                LOG_W(getName() << ": invalid end.");
                return;
            }
            m_valid = true;
        }

        ~DirectiveTypeTraits(void) {}

        void set(std::vector<ServerOpts>& opts) const {
            opts.push_back(ServerOpts());
        }

        void set(ServerOpts&) const {
            LOG_W(getName() << ": invalid function call.");
        }

        void set(LocationOpts&) const {
            LOG_W(getName() << ": invalid function call.");
        }

        bool isGlobalDirective(void) const { return (true); }
};

template<>
struct DirectiveTypeTraits<LineRoute> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(std::vector<std::string> const& args)
            : DirectiveTypeTraitsBase(LINE_LOCATION) {
            if (args.size() != 3) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive.");
                return;
            }
            if (args[2] != "{") {
                LOG_W(getName() << ": invalid end.");
                return;
            }
            m_target = (args[1][0] != '/') ? ("/" + args[1]) : args[1];
            m_valid = true;
        }

        ~DirectiveTypeTraits(void) {}

        void set(std::vector<ServerOpts>&) const {
            LOG_W(getName() << ": invalid function call.");
        }

        void set(ServerOpts& opts) const {
            for (std::vector<LocationOpts>::iterator it =
                     opts.m_locations.begin();
                 it != opts.m_locations.end(); ++it) {
                if (it->m_target == m_target) {
                    LOG_W(getName() << ": target " << m_target
                                    << " already exists. This route will be "
                                       "ignored.");
                    return;
                }
            }
            opts.m_locations.push_back(LocationOpts(m_target));
        }

        void set(LocationOpts&) const {
            LOG_W(getName() << ": invalid function call.");
        }

        bool isBlockDirective(void) const { return (true); }

        std::string m_target;
};

template<>
struct DirectiveTypeTraits<LineEnd> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(std::vector<std::string> const& args)
            : DirectiveTypeTraitsBase(LINE_END) {
            if (args.size() != 1) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive.");
                return;
            }
            m_valid = true;
        }

        DirectiveTypeTraits(std::vector<std::string> const& args,
                            std::vector<ServerOpts>&        opts)
            : DirectiveTypeTraitsBase(LINE_END) {
            if (args.size() != 1) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive.");
                return;
            }
            ServerOpts opt = opts.back();
            if (opt.m_host.empty() && opt.m_port.empty()) {
                LOG_W(
                    getName()
                    << ": trying to close block without listen directive set.");
                return;
            }
            if (std::string(opt.m_root).empty()) {
                LOG_W(getName() << ": trying to close block without root "
                                   "directive set.");
                return;
            }
            // checking for port:host:server_name duplicates
            for (std::vector<ServerOpts>::iterator it = opts.begin();
                 it != opts.end() - 1; ++it) {
                if (it->m_host == opt.m_host && it->m_port == opt.m_port &&
                    it->m_server_name == opt.m_server_name) {
                    LOG_W(getName()
                          << ": duplicate " << opt.m_port << ":" << opt.m_host
                          << ":" << opt.m_server_name << " combination.");
                    return;
                }
            }
            m_valid = true;
        }

        DirectiveTypeTraits(std::vector<std::string> const& args,
                            LocationOpts&                   opts)
            : DirectiveTypeTraitsBase(LINE_END) {
            if (args.size() != 1) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive.");
                return;
            }
            if (std::string(opts.m_root).empty()) {
                LOG_W(getName() << ": trying to close block without root "
                                   "directive set.");
                return;
            }
            m_valid = true;
        }

        ~DirectiveTypeTraits(void) {}

        void set(std::vector<ServerOpts>&) const {
            LOG_W(getName() << ": invalid function call.");
        }

        void set(ServerOpts&) const {
            LOG_W(getName() << ": invalid function call.");
        }

        void set(LocationOpts&) const {
            LOG_W(getName() << ": invalid function call.");
        }

        bool isBlockDirective(void) const { return (true); }

        bool isRouteDirective(void) const { return (true); }
};

template<>
struct DirectiveTypeTraits<LineListen> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(std::vector<std::string> const& args)
            : DirectiveTypeTraitsBase(LINE_LISTEN) {
            if (args.size() != 2) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive name.");
                return;
            }
            size_t pos = args[1].find(":");
            if (pos != std::string::npos) {
                if (!pos || pos == args[1].size() - 1) {
                    LOG_W(getName() << ": invalid host:port.");
                    return;
                }
                m_port = args[1].substr(0, pos);
                m_host = args[1].substr(pos + 1);
                if (!ft::string::isnumeric(m_port)) {
                    LOG_W(getName() << ": invalid host:port.");
                    return;
                }
            }
            else if (ft::string::isnumeric(args[1])) {
                m_host = "localhost";
                m_port = args[1];
            }
            else {
                m_port = "8080";
                m_host = args[1];
            }

            // resolv host name
            std::vector<std::string> resolvHost =
                net::Host(m_host).resolve(m_port);
            if (resolvHost.empty()) {
                LOG_W(getName() << ": failed to resolv host.");
                return;
            }
            m_host = resolvHost[0];

            m_valid = true;
        }

        void set(std::vector<ServerOpts>&) const {
            LOG_W(getName() << ": invalid function call.");
        }

        void set(ServerOpts& opts) const {
            opts.m_host = m_host;
            opts.m_port = m_port;
        }

        void set(LocationOpts&) const {
            LOG_W(getName() << ": invalid function call.");
        }

        ~DirectiveTypeTraits(void) {}

        bool isBlockDirective(void) const { return (true); }

        std::string m_host;
        std::string m_port;
};

template<>
struct DirectiveTypeTraits<LineServerName> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(std::vector<std::string> const& args)
            : DirectiveTypeTraitsBase(LINE_SERVER_NAME) {

            if (args.size() != 2) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive name.");
                return;
            }
            m_server_name = args[1];
            m_valid = true;
        }

        ~DirectiveTypeTraits(void) {}

        void set(std::vector<ServerOpts>&) const {
            LOG_W(getName() << ": invalid function call.");
        }

        void set(ServerOpts& opts) const { opts.m_server_name = m_server_name; }

        void set(LocationOpts&) const {
            LOG_W(getName() << ": invalid function call.");
        }

        bool isBlockDirective(void) const { return (true); }

        std::string m_server_name;
};

template<>
struct DirectiveTypeTraits<LineRoot> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(std::vector<std::string> const& args)
            : DirectiveTypeTraitsBase(LINE_ROOT) {
            if (args.size() != 2) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive name.");
                return;
            }
            m_root = args[1];
            if (!m_root.isValid()) {
                LOG_W(getName() << ": invalid root.");
                return;
            }
            m_valid = true;
        }

        ~DirectiveTypeTraits(void) {}

        void set(std::vector<ServerOpts>&) const {
            LOG_W(getName() << ": invalid function call.");
        }

        void set(ServerOpts& opts) const { opts.m_root = m_root; }

        void set(LocationOpts& opts) const { opts.m_root = m_root; }

        bool isBlockDirective(void) const { return (true); }

        bool isRouteDirective(void) const { return (true); }

        ft::directory m_root;
};

template<>
struct DirectiveTypeTraits<LineErrorPage> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(std::vector<std::string> const& args)
            : DirectiveTypeTraitsBase(LINE_ERROR_PAGE) {
            if (args.size() < 3) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive name.");
                return;
            }
            ft::file file = args.back();
            if (!file.isValid()) {
                LOG_W(getName() << ": invalid file.");
                return;
            }

            std::vector<std::string> error_codes =
                std::vector<std::string>(args.begin() + 1, args.end() - 1);

            for (std::vector<std::string>::iterator it = error_codes.begin();
                 it != error_codes.end(); ++it) {
                if (!ft::string::isnumeric(*it)) {
                    LOG_W(getName() << ": invalid error code.");
                    return;
                }
                m_error_pages[ft::string::stoi(*it)] = file;
            }

            m_valid = true;
        }

        ~DirectiveTypeTraits(void) {}

        void set(std::vector<ServerOpts>&) const {
            LOG_W(getName() << ": invalid function call.");
        }

        void set(ServerOpts& opts) const { opts.m_error_pages = m_error_pages; }

        void set(LocationOpts& opts) const {
            opts.m_error_pages = m_error_pages;
        }

        bool isBlockDirective(void) const { return (true); }

        bool isRouteDirective(void) const { return (true); }

        std::map<int, ft::file> m_error_pages;
};

template<>
struct DirectiveTypeTraits<LineMaxBodySize> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(std::vector<std::string> const& args)
            : DirectiveTypeTraitsBase(LINE_MAX_BODY_SIZE), m_max_body_size(0) {
            if (args.size() != 2) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive name.");
                return;
            }
            if (!ft::string::isnumeric(args[1])) {
                LOG_W(getName() << ": invalid max body size.");
                return;
            }
            m_max_body_size = ft::string::stoul(args[1]);
            m_valid = true;
        }

        ~DirectiveTypeTraits(void) {}

        void set(std::vector<ServerOpts>&) const {
            LOG_W(getName() << ": invalid function call.");
        }

        void set(ServerOpts& opts) const {
            opts.m_max_body_size = m_max_body_size;
        }

        void set(LocationOpts& opts) const {
            opts.m_max_body_size = m_max_body_size;
        }

        bool isBlockDirective(void) const { return (true); }

        bool isRouteDirective(void) const { return (true); }

        unsigned long m_max_body_size;
};

template<>
struct DirectiveTypeTraits<LineAllowMethods> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(std::vector<std::string> const& args)
            : DirectiveTypeTraitsBase(LINE_ALLOW_METHODS) {
            if (args.size() < 2) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive name.");
                return;
            }
            for (std::vector<std::string>::const_iterator it = args.begin() + 1;
                 it != args.end(); ++it) {
                if (http::convertMethod(*it) == http::UNKNOWN_METHOD) {
                    LOG_W(getName() << ": invalid method.");
                    return;
                }
                m_allowed_methods.insert(*it);
            }
            m_valid = true;
        }

        ~DirectiveTypeTraits(void) {}

        void set(std::vector<ServerOpts>&) const {
            LOG_W(getName() << ": invalid function call.");
        }

        void set(ServerOpts& opts) const {
            opts.m_allowed_methods = m_allowed_methods;
        }

        void set(LocationOpts& opts) const {
            opts.m_allowed_methods = m_allowed_methods;
        }

        bool isBlockDirective(void) const { return (true); }

        bool isRouteDirective(void) const { return (true); }

        std::set<std::string> m_allowed_methods;
};

template<>
struct DirectiveTypeTraits<LineIndex> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(std::vector<std::string> const& args)
            : DirectiveTypeTraitsBase(LINE_INDEX) {
            if (args.size() != 2) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive name.");
                return;
            }
            m_index = args[1];
            if (!m_index.isValid()) {
                LOG_W(getName() << ": invalid index file.");
                return;
            }
            m_valid = true;
        }

        ~DirectiveTypeTraits(void) {}

        void set(std::vector<ServerOpts>&) const {
            LOG_W(getName() << ": invalid function call.");
        }

        void set(ServerOpts&) const {
            LOG_W(getName() << ": invalid function call.");
        }

        void set(LocationOpts& ops) const { ops.m_index = m_index; }

        bool isRouteDirective(void) const { return (true); }

        ft::file m_index;
};

template<>
struct DirectiveTypeTraits<LineAutoIndex> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(std::vector<std::string> const& args)
            : DirectiveTypeTraitsBase(LINE_AUTOINDEX), m_autoindex(false) {
            if (args.size() != 2) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive name.");
                return;
            }
            if (args[1] == "on") { m_autoindex = true; }
            else if (args[1] == "off") { m_autoindex = false; }
            else {
                LOG_W(getName() << ": invalid autoindex value.");
                return;
            }
            m_valid = true;
        }

        ~DirectiveTypeTraits(void) {}

        void set(std::vector<ServerOpts>&) const {
            LOG_W(getName() << ": invalid function call.");
        }

        void set(ServerOpts&) const {
            LOG_W(getName() << ": invalid function call.");
        }

        void set(LocationOpts& ops) const { ops.m_autoindex = m_autoindex; }

        bool isRouteDirective(void) const { return (true); }

        bool m_autoindex;
};

template<>
struct DirectiveTypeTraits<LineCgiExtension> : public DirectiveTypeTraitsBase {

        DirectiveTypeTraits(std::vector<std::string> const& args)
            : DirectiveTypeTraitsBase(LINE_FASTCGI) {
            if (args.size() != 2) {
                LOG_W(getName() << ": invalid number of elements.");
                return;
            }
            if (args[0] != getName()) {
                LOG_W(getName() << ": invalid directive name.");
                return;
            }
            if (cgi::CgiHandler::convertCgiExtension(args[1]) == cgi::Unknown) {
                LOG_W(getName() << ": invalid cgi extension.");
                return;
            }
            m_cgi_extension = args[1];
            m_valid = true;
        }

        ~DirectiveTypeTraits(void) {}

        void set(std::vector<ServerOpts>&) const {
            LOG_W(getName() << ": invalid function call.");
        }

        void set(ServerOpts&) const {
            LOG_W(getName() << ": invalid function call.");
        }

        void set(LocationOpts& ops) const {
            ops.m_cgi_extension = m_cgi_extension;
        }

        bool isRouteDirective(void) const { return (true); }

        std::string m_cgi_extension;
};

} // namespace config

#endif /* CONFIG_DIRECTIVES_HPP */