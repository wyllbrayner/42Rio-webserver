#include "./Server.hpp"

Server::Server(int port, ConfigFile virtualServer) : \
	_fd_socket(-1), _port(port) , _serverConf(virtualServer) {}

Server & Server::operator=(const Server & src)
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

const int		&   Server::getSocket(void) const
{
	return (this->_fd_socket);
}

const int		&   Server::getPort(void) const
{
	return (this->_port);
}

const ConfigFile &   Server::getServerConf(void) const
{
	return (this->_serverConf);
}
bool	Server::initialize(void)
{
	int	value;

	value = 1;
	this->_fd_socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);//cria socket IPV4(AF_INET) TCP(SOCK_STREAM) NONBLOCK SOCK(SOCK_NONBLOCK)
//	if (this->_port == 8002) //para teste!!!
//		this->_fd_socket = -1;
	if (this->_fd_socket == -1)
	{
		std::cout << "Error creating socket." << std::endl;
		return (false);
	}
	setsockopt(this->_fd_socket, SOL_SOCKET, SO_REUSEADDR, \
				&value, sizeof(int)); //useful for quickly reusing a port in case of server failure. 
	// Bind the socket to a specific IP address and port
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(this->_port); //Reorganize order of bytes to network order.
	server_addr.sin_addr.s_addr = this->_serverConf.getHost();
	if (bind(this->_fd_socket, \
		reinterpret_cast<struct sockaddr*>(&server_addr), \
		sizeof(server_addr)) == -1)
	{ // choose a port to itself.
		std::cout << "Error binding socket" << std::endl;
		return (false);
	}
	// Listen for incoming connections
	if (listen(this->_fd_socket, 1024) == -1)
	{ //coloca o socket em modo de escuta para até 1024 requisições pendentes
		std::cout << "Error listening for connections." << std::endl;
		return (false);
	}
	return (true);
}
/*
*/

int	Server::acceptCon(void)	const
{
	// Accept incoming connections and get a file descriptor for reading and writing
	struct sockaddr_in	client_address;
	socklen_t			client_address_len;
	int					client_socket;

	client_address_len = sizeof(client_address);
	client_socket = accept(this->_fd_socket, \
		reinterpret_cast<struct sockaddr*>(&client_address), \
		&client_address_len);
	return (client_socket);
}

void	Server::closeCon(void)
{
//	std::cout << "início | closeCon _fd_socket: " << this->_fd_socket << std::endl;
	if (this->_fd_socket >= 0)
	{
		close(this->_fd_socket);
		this->_fd_socket = -1;
	}
//	std::cout << "fim    | closeCon" << std::endl;
}