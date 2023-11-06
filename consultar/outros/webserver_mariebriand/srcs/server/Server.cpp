/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 19:18:35 by vmoreau           #+#    #+#             */
/*   Updated: 2022/02/18 23:15:14 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Server.hpp"
#include "../cgi/CgiProcess.hpp"
#include "../response/Response.hpp"

std::string ipToString(unsigned int ip)
{
	unsigned char		v[4];
	std::ostringstream	os;

	for (int i = 0; i < 4; ++i)
	{
		v[i] = (ip >> (i * 8)) & 0xFF;
	}

	for (int i = 0; i < 3; ++i)
	{
		os << static_cast<int>(v[i]);
		os << '.';
	}
	os << static_cast<int>(v[3]);

	return (os.str());
}

int Server::server_is_alive = 1;								// Static int for server loop

Server::Server(/* args */)
{
}

Server::~Server()
{
	for (std::map< int, Request *>::iterator it = this->_client_sock.begin(); it != this->_client_sock.end(); it++)
	{
		close(it->first);
		delete(it->second);
	}
	this->Server_closeAllSocket();
	this->Server_Zero_all_set();
}

/*
	Those Functions are:
		Server_zero_all_set() -> set all fd_set to ZERO, they are reseted
		Server_closeSocket() -> close a fd/socket
		Server_closeAllSocket() -> Close all server fds/sockets
*/
void Server::Server_Zero_all_set()
{
	FD_ZERO(&this->_currentfds);
	FD_ZERO(&this->_readfds);
	FD_ZERO(&this->_writefds);
}

void Server::Server_closeSocket(int socket)
{
	if (socket >= 0)
		close(socket);
}

void Server::Server_closeAllSocket()
{
	for (std::map< int, sockaddr_in >::iterator it = this->_socket.begin(); it != this->_socket.end(); it++)
		this->Server_closeSocket(it->first);
	std::cout << YELLOW << "All opened Sockets have been closed.\n" << NC;
}

/*
	This fuction prepare all Server Socket, all server block have one socket
*/
void Server::Server_setSocket()
{
	for (size_t i = 0; i < this->_servers.size(); i++)
	{
		int	mysock;
		int opt = 1;										// opt = 1 -> allowed, opt = 0 disallowed

		memset((char *)&mysock, 0, sizeof(mysock));
		if ((mysock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			this->Server_closeAllSocket();
			throw ServerError("Socket creation error");
		}
		if (fcntl(mysock, F_SETFL, O_NONBLOCK) < 0)
			std::cout << "Fcntl failed. errno: " << errno << std::endl;

		if (setsockopt(mysock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)	// Set Socket Option Reuse address which allow which allows a socket to forcibly bind to a port used by another socket
		{
			this->Server_closeAllSocket();
			throw ServerError("Setsockopt \"SO_REUSEADDR\" error");
		}
		if (setsockopt(mysock, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0)	// Set Socket Option Reuse address which allow which allowsmultiple AF_INET sockets to be bound to an identical socket address.
		{
			this->Server_closeAllSocket();
			throw ServerError("Setsockopt \"SO_REUSEPORT\" error");
		}

		struct sockaddr_in		myaddr;

		int addr_size = sizeof(myaddr);
		memset((char *)&myaddr, 0, addr_size);
		myaddr.sin_family = AF_INET;
		myaddr.sin_port = htons(this->_servers[i].get_port());
		myaddr.sin_addr.s_addr = inet_addr(this->_servers[i].get_host().c_str());

		std::cerr << "IP: " << ipToString(myaddr.sin_addr.s_addr) << std::endl;
		std::cerr << "PORT: " << this->_servers[i].get_port() << std::endl;

		if (bind(mysock, (struct sockaddr *)&myaddr, addr_size) < 0)				// Assigns the address specified in addr to the socket referenced by the sockfd file descriptor
		{
			this->Server_closeAllSocket();
			throw ServerError(std::strerror(errno));
		}

		int	backlog = 100;						// Backlog is the maximum number of pending connections

		if (listen(mysock, backlog) < 0)		// Marks the socket referenced by sockfd as a passive socket, that mean as a socket that will be used to accept incoming connection requests using accept().
		{
			this->Server_closeAllSocket();
			throw ServerError("Couldn't listen to socket.");
		}
		this->_socket.insert(std::make_pair(mysock, myaddr));
	}
}

void Server::Server_setFd()
{
	FD_ZERO(&this->_currentfds);					// Set all FD in currentfds to ZERO, reset
	for (std::map< int, sockaddr_in >::iterator it = this->_socket.begin(); it != this->_socket.end(); it++)
		FD_SET(it->first, &this->_currentfds);		// Set all server socket binded in set socket, in currentfds as open server
	this->_nfds = this->_socket.rbegin()->first;	// nfds = numbers fds -> Max socket binded
}


void Server::Server_init(confpars *html, std::vector< serv_block > serv)
{
	this->_html = html;
	this->_servers = serv;
}

/*
	This function set read and write fd_set to the currentfds which contain all socket currently bind.
	Select will sort fd and leave only the fds whos asks a write or a read and return the number of ready fds
*/
void Server::Server_select()
{
	this->_readfds = this->_currentfds;
	this->_writefds = this->_currentfds;

	int ret_select = select(this->_nfds + 1, &this->_readfds, &this->_writefds, NULL, NULL);

	if (ret_select < 0 && (errno != EINTR))
		throw ServerError(std::strerror(errno));

	this->_rdy_fd = ret_select;
}

/*
	This function will loop on nfds and check if the socket is contain in readfds
	It will accept the connection
	And create an open socket in currentfds to saved them until the communication is terminated with this last one
*/
void Server::Server_loopServ()
{
	for (int i = 0; i <= this->_nfds; i++)
	{
		if (FD_ISSET(i, &this->_readfds))
		{
			this->_rdy_fd--;
			if (this->_socket.find(i) != this->_socket.end())
			{
				std::map<int, sockaddr_in>::iterator it = this->_socket.find(i);
				int client_socket = accept(it->first, (struct sockaddr*)NULL, NULL);
				if (client_socket < 0 && errno != EAGAIN)
				{
					std::cerr << client_socket << "  " << EAGAIN << '\n';
					std::cerr << RED << "Server error: " << NC << "Couldn't accept connection.\n";
					std::cerr << std::strerror(errno);
					exit(EXIT_FAILURE);
				}
				else if (client_socket > 0)
				{
					std::cout << YELLOW << "New incoming connection (fd " << client_socket << ")" << " for -> " << i << NC << std::endl;
					FD_SET(client_socket, &this->_currentfds);

					Request* req = new Request(client_socket, &this->_servers[i - 3]);

					this->_client_sock.insert(std::make_pair(client_socket, req));

					if (client_socket > this->_nfds)
						this->_nfds = client_socket;
				}
			}
		}
	}
}

/*
	This function will loop on all client socket set in the loopServer
	If socket is found in readfds
		then a request is recv and treated
		if the request is ready then a response to this request can be created
	If socket is found in writefds and a response has been written
		then this response is send to the socket
		as send() can not send all the response in one time we erase only the content who have been send
		if the response is empty, it mean that all the response was sent and then the connexion with this socket is finished
			then the socket is clear in currentfds, fd is closed, request class is delete and the socket is erase in client_socket
*/

void Server::Server_loopClient()
{
	for (std::map<int, Request*>::iterator it = this->_client_sock.begin(); it != this->_client_sock.end() && this->_rdy_fd > 0; )
	{
		int ret = 0;
		if (FD_ISSET(it->first, &this->_readfds))
		{
			ret = it->second->parse();

			if (it->second->is_request_ready() == true && it->second->is_connection_end() == false)
			{
				if (ret >= 0)
					it->second->parseBuf();
				else
					it->second->set_error_recv();

				std::cout << CYAN "\nRequest after parsing:\n" << *(it->second) << NC;

				CgiProcess newProcess(it->second, this);
				bool is_cgi_needed = false;
				try
				{
					if (newProcess.isCgiNeeded() == true)
					{
						newProcess.init();
						if (it->second->returnStatusCode() < 400)
						{
							if (newProcess.exeCgiProgram() == -1 || newProcess.get_cgiOutput().empty() == true)
								it->second->setError(500);
							else
								is_cgi_needed = true;
						}
						else
							is_cgi_needed = false;
				}
				}
				catch(const std::exception& e)
				{
					it->second->setError(500);
					std::cerr << RED << "ERROR: " << NC << e.what() << '\n';
				}

				Response	resp(*it->second, it->second->getBlock(), is_cgi_needed, newProcess.get_cgiOutput());
				this->_response = resp.getVecResponse();
			}
		}
		if (FD_ISSET(it->first, &this->_writefds) && this->_response.size())
		{
			// print_response(-1);

			ret = send(it->first, &this->_response[0], this->_response.size() , MSG_DONTWAIT);

			// std::cout << this->_response.size() << " AFTER SEND: " << ret << NC << std::endl;

			if (ret == -1)
			{
				std::cout << RED << "ERROR: " << NC << " send() Couldn't be able to send the response\n";
				ret = this->_response.size();
			}

			this->_response.erase(this->_response.begin(), this->_response.begin() + ret);
			if (this->_response.size() == 0)
			{
				std::cout << RED << "connection closed(fd " << it->first << ")\n" << NC << std::endl;
				FD_CLR(it->first, &this->_currentfds);
				close(it->first);
				delete(it->second);
				std::map<int, Request*>::iterator tmp = it;
				it++;
				this->_client_sock.erase(tmp);
			}
			continue;
		}
		it++;
	}
}

void Server::Server_launch()
{
	this->_rdy_fd = 0;
	Server_setSocket();
	Server_setFd();

	while (Server::server_is_alive)
	{
		this->Server_select();
		this->Server_loopServ();
		this->Server_loopClient();
	}
}


// --------------- DEBUG --------------- //

/*
	Those Functions are:
		printfds() -> Print all socket ready in each fd_set
		print_response() -> Print the response from response start to response n
*/
void Server::print_fds(const char *color)
{
	std::cout << color << "\nFD_SET in current:\n";
	for (int i = 0; i <= this->_nfds; i++)
	{
		if (FD_ISSET(i, &this->_currentfds))
			std::cout << i << ' ';
	}
	std::cout << '\n';
		std::cout << "FD_SET in read:\n";
	for (int i = 0; i <= this->_nfds; i++)
	{
		if (FD_ISSET(i, &this->_readfds))
			std::cout << i << ' ';
	}
	std::cout << '\n';
		std::cout << "FD_SET in write:\n";
	for (int i = 0; i <= this->_nfds; i++)
	{
		if (FD_ISSET(i, &this->_writefds))
			std::cout << i << ' ';
	}
	std::cout << "\n\n" << NC;
}

void Server::print_response(int n)
{
	int size = n;
	if (size == -1)
		size = this->_response.size();

	std::vector<unsigned char> tmp_resp(this->_response.begin(), this->_response.begin() + size);

	for (std::vector<unsigned char>::iterator it = tmp_resp.begin(); it != tmp_resp.end(); it++)
		std::cout << BLUE << *it;
	std::cout << NC << '\n';
}
