/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 16:53:05 by vmoreau           #+#    #+#             */
/*   Updated: 2022/02/21 10:26:17 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include <fstream>
#include <string>

// *INCLUDE FOR FUNCTION AUTORIZED* //

#include <cstdlib>			// malloc / free /
#include <unistd.h>			// write /
#include <sys/socket.h>		// socket / accept / listen / send / recv / bind / setsockopt / getsockname /
#include <netinet/in.h>
#include <arpa/inet.h>		// htons / htonl / ntohs / ntohl / inet_adrr
#include <sys/select.h> 	// select /
#include <poll.h>			// poll /
#include <fcntl.h>			// fcntl, SEUL UTILISATION POSSIBLE = fcntl(fd, F_SETFL, O_NONBLOCK);

// #include <string_view>
#include <map>
#include <bitset>
#include <sstream>
#include <ctime>

// #include <sys/types.h>
// #include <netinet/in.h>

#include <sys/types.h>
#include <sys/dir.h>
#include <dirent.h>
#include "sys/stat.h"

// *UTILS* //

#include "color.hpp"

#define CONF_DEFAULT_PATH "./conf.d/default/default.conf"
#define DEFAULT_ERR_400 "./html/error/400.html"
#define DEFAULT_ERR_404 "./html/error/404.html"
#define DEFAULT_ERR_4xx "./html/error/4xx.html"
#define DEFAULT_ERR_500 "./html/error/500.html"
#define DEFAULT_ERR_5xx "./html/error/5xx.html"
#define DEFAULT_ROOT "html"
#define DEFAULT_PATH "index.html"
#define	DEFAULR_SERV_NAME "default_serv"
#endif
