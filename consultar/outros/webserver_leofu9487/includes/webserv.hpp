#pragma once

// C library

#include <arpa/inet.h> // htons, htonl, ntohs, ntohl, inet_addr
#include <sys/select.h> // select
#include <poll.h> // poll
#include <sys/socket.h> // socket, accept, listen, send, recv, bind, connect, setsockopt, getsockname
#include <fcntl.h> // fcntl
#include <unistd.h> // read, write, close
#include <dirent.h>
#include <sys/wait.h>

#ifdef __linux__
	#include <sys/epoll.h> // epoll
#else
	#include <sys/event.h> // kqueue
#endif

// C++ library

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <map>
#include <cstring>
#include <stack>

// *.hpp

#include "ServerInfo.hpp"
#include "exception.hpp"
#include "utils.hpp"
#include "conf.hpp"
#include "socket.hpp"
#include "connect.hpp"
#include "MACROS.hpp"
#include "ClientInfo.hpp"
#include "http.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "Autoindex.hpp"
#include "UploadFile.hpp"
#include "Cgi.hpp"
