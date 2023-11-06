#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "config/Parser.hpp"
#include "server/Server.hpp"

#include <string>

namespace webserv {

void webserv(std::string const& filename);

} // namespace webserv

#endif /* WEBSERV_HPP */