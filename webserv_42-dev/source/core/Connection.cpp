#include "Connection.hpp"

Connection::Connection()
{
}

Connection::~Connection()
{
}

void	Connection::addNewSocket(int socket_fd) {
	std::cout << "Adding new socket number " << socket_fd << std::endl;
	// Set the socket to be non-blocking
	fcntl(socket_fd, F_SETFL, O_NONBLOCK);

	pollfd pfd;
	pfd.fd = socket_fd;
	pfd.events = POLLIN | POLLOUT;
	pfd.revents = 0;
	this->poolAllFd.push_back(pfd);

}

void	Connection::addClientSocket(int socket) {
	std::cout << "Creating conection with a new client" << std::endl;
	this->addNewSocket(socket);
}

void	Connection::addServersSockets(std::vector<Server> const& servers) {
	std::cout << "Adding servers sockets" << std::endl;
	for (size_t i = 0; i < servers.size(); i++)
	{
		addNewSocket(servers[i]._fd_socket);
	}
}

void	Connection::closeConnection(int index) {
	std::cerr << "Closing the connection: " << this->poolAllFd[index].fd << std::endl;
	close(this->poolAllFd[index].fd);
	this->poolAllFd.erase(this->poolAllFd.begin() + index);
}

std::vector<pollfd>&  Connection::getPollFd() {
    return (this->poolAllFd);
}

pollfd const&             Connection::getFd(int i) {
    return (this->poolAllFd[i]);
}

