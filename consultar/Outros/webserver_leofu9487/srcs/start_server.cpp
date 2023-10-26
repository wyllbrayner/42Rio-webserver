#include "../includes/webserv.hpp"

#ifdef __linux__

void	start_server(std::map<int, ServerInfo *> &fd_of_servers)
{
	int	epoll_fd = create_epoll(fd_of_servers);
	struct epoll_event	eventlist[MAX_EVENTS];
	std::map<int, ClientInfo> fd_of_clients;

	for (;;) // infinite loop for new event
	{
		int event_amount = epoll_wait(epoll_fd, eventlist, MAX_EVENTS, -1);
		if (event_amount == -1)
			throw(FailToWaitEpoll());
		for (int i = 0 ; i < event_amount ; ++i)
		{
			if ((eventlist[i].events & EPOLLRDHUP)
				|| (eventlist[i].events & EPOLLHUP)
				|| (eventlist[i].events & EPOLLERR)) // client quit
				delete_client_from_epoll(fd_of_clients, epoll_fd, eventlist[i].data.fd);
			else if (fd_of_servers.find(eventlist[i].data.fd) != fd_of_servers.end()) // event of server means new client connected
				accept_new_client(epoll_fd, eventlist[i].data.fd, fd_of_clients, fd_of_servers);
			else if (eventlist[i].events & EPOLLIN) // there is new input from old client
			{
				std::map<int, ClientInfo>::iterator it = fd_of_clients.find(eventlist[i].data.fd);
				if (it == fd_of_clients.end())
					throw(ClientNotInMap());
				if (read_request(it) <= 0) // no request sent
					delete_client_from_epoll(fd_of_clients, epoll_fd, eventlist[i].data.fd);
				else
				{
					deal_with_request(it);
					delete_client_from_epoll(fd_of_clients, epoll_fd, eventlist[i].data.fd);
				}
			}
			else // Neet to deal with Write Event ?
			{
				delete_client_from_epoll(fd_of_clients, epoll_fd, eventlist[i].data.fd);
			}
		}
	}
}

#else

void	start_server(std::map<int, ServerInfo *> &fd_of_servers)
{
	int	kqueue_fd = create_kqueue(fd_of_servers);
	struct kevent	eventlist[MAX_EVENTS];
	std::map<int, ClientInfo> fd_of_clients;

	for (;;)
	{
		int event_amount = kevent(kqueue_fd, NULL, 0, eventlist, MAX_EVENTS, NULL);
		if (event_amount == -1)
			throw(FailToWaitKqueue());
		for (int i = 0; i < event_amount; ++i)
		{
			if (eventlist[i].flags & EV_EOF)
				delete_client_from_kqueue(fd_of_clients, eventlist[i].ident);
			else if (fd_of_servers.find(eventlist[i].ident) != fd_of_servers.end())
				accept_new_client(kqueue_fd, eventlist[i].ident, fd_of_clients, fd_of_servers);
			else if (eventlist[i].filter == EVFILT_READ)
			{
				std::map<int, ClientInfo>::iterator it = fd_of_clients.find(eventlist[i].ident);
				if (it == fd_of_clients.end())
					throw(ClientNotInMap());
				if (read_request(it) <= 0)
					delete_client_from_kqueue(fd_of_clients, eventlist[i].ident);
				else
				{
					// check if request is complete
					// if is complete : construct response, put in write event
					if (it->second.request_is_complete())
					{
						deal_with_request(it);
						delete_client_from_kqueue(fd_of_clients, eventlist[i].ident);
					}
				}
			}
			// else if (eventlist[i].filter == EVFILT_WRITE)
			// {

			// 	// send response, if not complete : put in write event
			// 	// else delete client (maybe print_log)
			// }
			else
				delete_client_from_kqueue(fd_of_clients, eventlist[i].ident);
		}
	}
}

#endif
