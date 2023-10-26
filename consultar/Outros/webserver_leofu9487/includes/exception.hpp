#pragma once
#include "webserv.hpp"

/*
Conf File
*/

class InvalidConfFilePath : public std::exception
{
	public:
		char const	*what() const throw()
		{
			return "Invalid Configuration File Path";
		}
};

class InvalidConfFile : public std::exception
{
	public:
		char const	*what() const throw()
		{
			return "Invalid Configuration File";
		}
};

class ConfFileParseError : public std::exception
{
	private:
		std::string	_msg;

	public:
		ConfFileParseError(std::string msg)
		:	_msg(msg) {}

		~ConfFileParseError() throw() {};

		char const	*what() const throw()
		{
			std::cerr << "Configuration File Parse Error" << std::endl;
			return _msg.c_str();
		}
};

/*
Socket
*/

class FailToCreateSocket : public std::exception
{
	public:
		char const	*what() const throw()
		{
			return "Fail To Create Socket";
		}
};

class FailToSetSocketOption : public std::exception
{
	public:
		char const	*what() const throw()
		{
			return "Fail To Set Socket Option";
		}
};

class FailToBind : public std::exception
{
	public:
		char const	*what() const throw()
		{
			return "Fail To Bind";
		}
};

class FailToListen : public std::exception
{
	public:
		char const	*what() const throw()
		{
			return "Fail To Listen";
		}
};

/*
Connection
*/

class FailToCreateEpoll : public std::exception
{
	public:
		char const	*what() const throw()
		{
			return "Fail To Create Epoll";
		}
};

class FailToControlEpoll : public std::exception
{
	public:
		char const	*what() const throw()
		{
			return "Fail To Control Epoll";
		}
};

class FailToWaitEpoll : public std::exception
{
	public:
		char const	*what() const throw()
		{
			return "Fail To Wait Epoll";
		}
};

class FailToAccept : public std::exception
{
	public:
		char const	*what() const throw()
		{
			return "Fail To accept";
		}
};

class ClientNotInMap : public std::exception
{
	public:
		char const	*what() const throw()
		{
			return "Client Not In Map";
		}
};

class FailToCreateKqueue : public std::exception
{
	public:
		char const	*what() const throw()
		{
			return "Fail To Create Kqueue";
		}
};

class FailToControlKqueue : public std::exception
{
	public:
		char const	*what() const throw()
		{
			return "Fail To Control Kqueue";
		}
};

class FailToWaitKqueue : public std::exception
{
	public:
		char const	*what() const throw()
		{
			return "Fail To Wait Kqueue";
		}
};

/*
HTTP
*/



/*
Other
*/
class FailToGetFdStatus : public std::exception
{
	public:
		char const	*what() const throw()
		{
			return "Fail To Get fd status";
		}
};

class FailToSetFdStatus : public std::exception
{
	public:
		char const	*what() const throw()
		{
			return "Fail To Set fd status";
		}
};

