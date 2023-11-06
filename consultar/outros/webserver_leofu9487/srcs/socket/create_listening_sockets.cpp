#include "../../includes/webserv.hpp"

static int	create_one_listening_socket(ServerInfo &info)
{
	int	socket_fd, enabled = 1;
	struct sockaddr_in sockaddr;
	
	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw(FailToCreateSocket());
	print_log("Server " + info.get_name() + " created");
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(enabled)) == -1)
		throw(FailToSetSocketOption());
	
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(info.get_port());
	sockaddr.sin_addr.s_addr = inet_addr(info.get_IP().c_str());
	if (bind(socket_fd, reinterpret_cast<struct sockaddr *>(&sockaddr), sizeof(sockaddr)) == -1)
		throw(FailToBind());

	make_socket_nonblock(socket_fd); // don't know if this is correct

	if (listen(socket_fd, MAX_CLIENT))
		throw(FailToListen());
	
	return socket_fd;
}

void	create_listening_sockets(std::map<int, ServerInfo *> &result, std::vector<ServerInfo>& serverlist)
{
	for (unsigned i = 0 ; i < serverlist.size() ; ++i)
	{
		result[create_one_listening_socket(serverlist[i])] = &serverlist[i];
	}
}
