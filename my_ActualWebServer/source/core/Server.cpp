#include "./Server.hpp"

Server::Server(int port, ConfigFile virtualServer) : \
	_fd_socket(-1), _port(port) , _serverConf(virtualServer) {}

Server				&	Server::operator=(const Server & src)
{
	if (this != &src)
	{
		this->_fd_socket = src.getSocket();
		this->_port = src._port;
		this->_serverConf = src.getServerConf();
	}
	return (*this);
}

Server::Server(const Server & copy)
{
	*this = copy;
	return ;
}

Server::~Server(void)
{
	_fd_socket = -1;
	_port = 0;
}

const int			&	Server::getSocket(void) const
{
	return (this->_fd_socket);
}

const int			&	Server::getPort(void) const
{
	return (this->_port);
}

const ConfigFile	&	Server::getServerConf(void) const
{
	return (this->_serverConf);
}

bool					Server::initialize(void)
{
	struct sockaddr_in	server_addr;
	int					value;

	value = 1;
	this->_fd_socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (this->_fd_socket == -1)
	{
		std::cout << "Error creating socket." << std::endl;
		return (false);
	}
	setsockopt(this->_fd_socket, SOL_SOCKET, SO_REUSEADDR, \
				&value, sizeof(int));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(this->_port);
	server_addr.sin_addr.s_addr = this->_serverConf.getHost();
	if (bind(this->_fd_socket, \
		reinterpret_cast<struct sockaddr*>(&server_addr), \
		sizeof(server_addr)) == -1)
	{
		std::cout << "Error binding socket" << std::endl;
		return (false);
	}
	if (listen(this->_fd_socket, 1024) == -1)
	{
		std::cout << "Error listening for connections." << std::endl;
		return (false);
	}
	return (true);
}

int						Server::acceptCon(void) const
{
	struct sockaddr_in	client_address;
	socklen_t			client_address_len;
	int					client_socket;

	client_address_len = sizeof(client_address);
	client_socket = accept(this->_fd_socket, \
		reinterpret_cast<struct sockaddr*>(&client_address), \
		&client_address_len);
	return (client_socket);
}

void					Server::closeCon(void)
{
	if (this->_fd_socket >= 0)
	{
		close(this->_fd_socket);
		this->_fd_socket = -1;
	}
}