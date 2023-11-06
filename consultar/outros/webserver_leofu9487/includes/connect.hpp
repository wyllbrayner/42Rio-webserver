#pragma once
#include "webserv.hpp"
#include "ClientInfo.hpp"

void	start_server(std::map<int, ServerInfo *> &fd_of_servers);
void	close_sockets(std::map<int, ServerInfo *> &fd_of_servers);

#ifdef __linux__

void	add_read_event_in_epoll(int epoll_fd, int socket_fd);
void	delete_client_from_epoll(std::map<int, ClientInfo> &fd_map, int epoll_fd, int target_fd);
void	accept_new_client(int epoll_fd, int server_fd, std::map<int, ClientInfo> &fd_of_clients, std::map<int, ServerInfo*> &fd_of_servers);
int	read_request(std::map<int, ClientInfo>::iterator &it);
int	create_epoll(std::map<int, ServerInfo *> &fd_of_servers);

#elif __APPLE__
int		create_kqueue(std::map<int, ServerInfo *> &fd_of_servers);
void	add_read_event_in_kqueue(int kqueue_fd, int socket_fd);
void	delete_client_from_kqueue(std::map<int, ClientInfo> &fd_map, int target_fd);
void	accept_new_client(int kqueue_fd, int server_fd, std::map<int, ClientInfo> &fd_of_clients, std::map<int, ServerInfo*> &fd_of_servers);
int		read_request(std::map<int, ClientInfo>::iterator &it);

#endif
