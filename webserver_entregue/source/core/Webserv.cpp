#include "./Webserv.hpp"

Webserv::Webserv(const std::vector<Server> & newServers) : \
		_servers(newServers)
{
	this->_nbrServers = _servers.size();
	this->start();
}

Webserv		&	Webserv::operator=(const Webserv & src)
{
	if (this != &src)
	{
		this->_nbrServers = src._nbrServers;
		this->_servers = src._servers;
		this->_conn = src._conn;
		this->_requests = src._requests;
	}
	return (*this);
}

Webserv::Webserv(const Webserv & copy)
{
	*this = copy;
	return ;
}

Webserv::~Webserv(void)
{
	this->_nbrServers = 0;
}

static void		printError(const std::string & error)
{
	std::cout << "\033[1;31m" << error << "\033[0m" << std::endl;
}

void			Webserv::start(void)
{
	this->_conn.addServersSockets(this->_servers);
	while (Utils::_serverRunning)
	{
		if (!updateStatusPoll())
			return;
		for (size_t i = 0; i < this->_conn.getPollFd().size(); i++)
		{
			if (this->isAbleToRead(i))
				this->readDataClient(i);
			else if (this->isAbleToWrite(i))
				this->sendDataClient(i);
			else if (this->isPollError(i))
				printError("Error for poll revents");
		}
	}
}

bool			Webserv::updateStatusPoll(void)
{
	ssize_t	status;

	status = poll(this->_conn.getPollFd().data(), \
					this->_conn.getPollFd().size(), -1);
	if (status == -1)
	{
		printError("Error in poll: ");
		this->_conn.closeAllConnections();
		return (false);
	}
	return (true);
}

bool			Webserv::isAbleToRead(int i)
{
	return (this->_conn.getFd(i).revents & POLLIN);
}

bool			Webserv::isAbleToWrite(int i)
{
	return (this->_conn.getFd(i).revents & POLLOUT);
}

bool			Webserv::isPollError(int i)
{
	return ((this->_conn.getFd(i).revents & POLLERR)
		|| (this->_conn.getFd(i).revents & POLLHUP)
		|| (this->_conn.getFd(i).revents & POLLNVAL));
}

void			Webserv::readDataClient(const int & i)
{
	int	clientWithMessage;
	int	idRequest;

	idRequest = i - this->_nbrServers;
	if (this->isRequestFromServer(i))
		this->openNewConnection(i);
	else if (!this->_requests[idRequest].isReady())
	{
		this->_requests[idRequest].setStart_time(time(NULL));
		clientWithMessage = this->_conn.getFd(i).fd;
		if (!this->_requests[idRequest].receiveFromClient(clientWithMessage))
		{
			this->_conn.closeConnection(i);
			this->_requests[idRequest].reset();
			this->_requests.erase(this->_requests.begin() + idRequest);
		}
	}
}

bool			Webserv::isRequestFromServer(int i)
{
	return (i < this->_nbrServers);
}

void			Webserv::openNewConnection(int i)
{
	int	newClient;

	newClient = this->_servers[i].acceptCon();
	this->_conn.addClientSocket(newClient);
	Request newRequest(newClient, this->_servers[i].getServerConf());
	this->_requests.push_back(newRequest);
}

void			Webserv::sendDataClient(const int & i)
{
	Response	response;
	int			idRequest;

	idRequest = i - this->_nbrServers;
	if (this->_requests[idRequest].isReady())
	{
		Client	client(this->_requests[idRequest], response);
		send(this->_conn.getFd(i).fd, response.httpMessage.c_str(), \
				response.httpMessage.size(), MSG_NOSIGNAL);
		this->_conn.closeConnection(i);
		this->_requests[idRequest].reset();
		this->_requests.erase(this->_requests.begin() + idRequest);
	}
}