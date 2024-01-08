#include "Connection.hpp"

Connection::Connection() {}

Connection::~Connection() {}

void	Connection::addServersSockets(std::vector<Server> const& servers)
{
//	std::cout << "Adding servers sockets" << std::endl;
	for (size_t i = 0; i < servers.size(); i++)
	{
		addNewSocket(servers[i].getSocket());
	}
}

void	Connection::addClientSocket(int socket)
{
	std::cout << "Creating conection with a new client" << std::endl;
	this->addNewSocket(socket);
}

void	Connection::addNewSocket(int socket_fd)
{
	std::cout << "Adding new socket number " << socket_fd << std::endl;
	// Set the socket to be non-blocking
	pollfd pfd;

	fcntl(socket_fd, F_SETFL, O_NONBLOCK);
	pfd.fd = socket_fd;
	pfd.events = POLLIN | POLLOUT;
	pfd.revents = 0;
	this->poolAllFd.push_back(pfd);
}

void	Connection::closeConnection(int index)
{
	std::cout << "Closing the connection: " << this->poolAllFd[index].fd \
	<< std::endl;
	close(this->poolAllFd[index].fd);
	this->poolAllFd.erase(this->poolAllFd.begin() + index);
}

void	Connection::closeAllConnections(void)
{
	size_t	i;

	i = 0;
	while (i < this->poolAllFd.size())
	{
		std::cout << "Closing the connection: " << this->poolAllFd[i].fd \
		<< std::endl;
		close(this->poolAllFd[i++].fd);
	}
	this->poolAllFd.clear();
}

std::vector<pollfd>	&	Connection::getPollFd(void)
{
    return (this->poolAllFd);
}

const pollfd		&	Connection::getFd(int i)
{
    return (this->poolAllFd[i]);
}