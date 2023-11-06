#ifdef __linux__
# include "../../includes/webserv.hpp"

int	create_epoll(std::map<int, ServerInfo *> &fd_of_servers)
{
	int epoll_fd = epoll_create(1);
	if (epoll_fd == -1)
		throw(FailToCreateEpoll());
	for (std::map<int, ServerInfo *>::iterator it = fd_of_servers.begin() ; it != fd_of_servers.end() ; ++it)
		add_read_event_in_epoll(epoll_fd, it->first);
	return (epoll_fd);
}

void	add_read_event_in_epoll(int epoll_fd, int socket_fd)
{
	struct epoll_event	event;

	std::memset(&event, 0, sizeof(event));
	event.events = EPOLLIN;
	event.data.fd = socket_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event) == -1)
		throw(FailToControlEpoll());
}

void	delete_client_from_epoll(std::map<int, ClientInfo> &fd_map, int epoll_fd, int target_fd)
{
	// if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, target_fd, NULL) == -1)
	// 	throw(FailToControlEpoll());
	if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, target_fd, NULL) == -1)
		print_log("One client fail to disconnect");
	else
		print_log("One client disconnected successfully");
	close(target_fd);
	fd_map.erase(target_fd);
}

void	accept_new_client(int epoll_fd, int server_fd, std::map<int, ClientInfo> &fd_of_clients, std::map<int, ServerInfo*> &fd_of_servers)
{
	struct sockaddr_in	sockaddr;
	socklen_t	len = sizeof(sockaddr);

	int client_fd = accept(server_fd, reinterpret_cast<struct sockaddr*>(&sockaddr), &len);
	if (client_fd < 0)
	{
		print_log("One client fail to connect");
		return ;
	}
	print_log("One client connected successfully");
	make_socket_nonblock(client_fd); // is it correct ?
	add_read_event_in_epoll(epoll_fd, client_fd);
	fd_of_clients.insert(std::make_pair(client_fd, ClientInfo((*(fd_of_servers[server_fd])))));
}

int	read_request(std::map<int, ClientInfo>::iterator &it)
{
	int	ret = 0;
	int	len;
	std::string str;

	while (1)
	{
		char	buf[BUFFER_SIZE];
		len = recv(it->first, buf, BUFFER_SIZE - 1, 0);
		if (len <= 0)
			break ;
		ret += len;
		buf[len] = 0;
		str += buf;
	}
	if (ret > 0)
		it->second.add_request(str);
	return ret;
}

#endif