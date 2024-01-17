#include "Connection.hpp"

Connection::Connection(void) {}

Connection			&	Connection::operator=(const Connection & src)
{
	if (this != &src)
		this->_poolAllFd = src._poolAllFd;
	return (*this);
}

Connection::Connection(const Connection & copy)
{
	*this = copy;
	return ;
}

Connection::~Connection(void) {}

void					Connection::addServersSockets(std::vector<Server> \
						const& servers)
{
	for (size_t i = 0; i < servers.size(); i++)
		addNewSocket(servers[i].getSocket());
}

void					Connection::addClientSocket(int socket)
{
	std::cout << "Creating conection with a new client" << std::endl;
	this->addNewSocket(socket);
}

void					Connection::addNewSocket(int socket_fd)
{
	pollfd	pfd;

	std::cout << "Adding new socket number " << socket_fd << std::endl;
	fcntl(socket_fd, F_SETFL, O_NONBLOCK);
	pfd.fd = socket_fd;
	pfd.events = POLLIN | POLLOUT;
	pfd.revents = 0;
	this->_poolAllFd.push_back(pfd);
}

void					Connection::closeConnection(int index)
{
	std::cout << "Closing the connection: " << this->_poolAllFd[index].fd \
	<< std::endl;
	close(this->_poolAllFd[index].fd);
	this->_poolAllFd.erase(this->_poolAllFd.begin() + index);
}

void					Connection::closeAllConnections(void)
{
	size_t	i;

	i = 0;
	while (i < this->_poolAllFd.size())
	{
		std::cout << "Closing the connection: " << this->_poolAllFd[i].fd \
		<< std::endl;
		close(this->_poolAllFd[i++].fd);
	}
	this->_poolAllFd.clear();
}

std::vector<pollfd>	&	Connection::getPollFd(void)
{
	return (this->_poolAllFd);
}

const pollfd		&	Connection::getFd(int i)
{
	return (this->_poolAllFd[i]);
}