/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 19:18:32 by vmoreau           #+#    #+#             */
/*   Updated: 2022/02/15 17:49:53 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "../incs/webserv.hpp"
#include "../conf/confpars.hpp"
#include "../request/Request.hpp"
#include "../response/Response.hpp"

class Server
{
private:

	fd_set							_currentfds;
	fd_set							_readfds;
	fd_set							_writefds;
	int								_nfds;

	std::vector< serv_block >		_servers;
	confpars 						*_html;
	std::map< int, sockaddr_in >	_socket;
	std::map< int, Request * >		_client_sock;
	int								_rdy_fd;
	std::vector<unsigned char>	_response;

	void Server_setSocket();
	void Server_setFd();
	void Server_Zero_all_set();
	void Server_closeSocket(int socket);
	void Server_closeAllSocket();
	void Server_select();
	void Server_loopServ();
	void Server_loopClient();

	void print_fds(const char *color);
	void print_response(int n);
public:
	static int server_is_alive;
	Server(/* args */);
	~Server();

	void Server_init(confpars *html, std::vector< serv_block > serv);
	void Server_launch();

	confpars	*get_conf() { return (this->_html); }

	class ServerError : public std::exception
	{
		private:
			std::string _msg;
		public:
			ServerError(std::string msg) : _msg(msg){}
			virtual ~ServerError() throw() {}
			virtual const char *what() const throw() {return (this->_msg.c_str());}
	};
};

#endif
