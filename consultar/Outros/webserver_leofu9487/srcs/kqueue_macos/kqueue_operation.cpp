/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kqueue_operation.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfu <yfu@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 11:29:54 by xli               #+#    #+#             */
/*   Updated: 2021/12/08 16:48:34 by yfu              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef __linux__
# include "../../includes/webserv.hpp"

int	create_kqueue(std::map<int, ServerInfo *> &fd_of_servers)
{
	int kqueue_fd = kqueue();
	if (kqueue_fd == -1)
		throw(FailToCreateKqueue());
	for (std::map<int, ServerInfo *>::iterator it = fd_of_servers.begin() ; it != fd_of_servers.end() ; ++it)
		add_read_event_in_kqueue(kqueue_fd, it->first);
	return (kqueue_fd);
}

void	add_read_event_in_kqueue(int kqueue_fd, int socket_fd)
{
	struct kevent	event;

	std::memset(&event, 0, sizeof(event));
	EV_SET(&event, socket_fd, EVFILT_READ, EV_ADD, 0, 0, 0);
	if (kevent(kqueue_fd, &event, 1, NULL, 0, NULL) == -1)
		throw(FailToControlKqueue());
}

void	add_write_event_in_kqueue(int kqueue_fd, std::map<int, ClientInfo>::iterator & it)
{
	struct kevent	event;
	// it->first = fd, it->second = client

	std::memset(&event, 0, sizeof(event));
	EV_SET(&event, it->first, EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, 0);
	if (kevent(kqueue_fd, &event, 1, NULL, 0, NULL) == -1)
		throw(FailToControlKqueue());
	it->second.set_ready();
}

void	delete_client_from_kqueue(std::map<int, ClientInfo> &fd_map, int target_fd)
{
	print_log("One client disconnected successfully");
	close(target_fd);
	fd_map.erase(target_fd);
}

void	accept_new_client(int kqueue_fd, int server_fd, std::map<int, ClientInfo> &fd_of_clients, std::map<int, ServerInfo*> &fd_of_servers)
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
	make_socket_nonblock(client_fd);
	add_read_event_in_kqueue(kqueue_fd, client_fd);
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
