#pragma once
#include "webserv.hpp"

void	create_listening_sockets(std::map<int, ServerInfo *> &result, std::vector<ServerInfo>& serverlist);
