#pragma once
#include "webserv.hpp"
class ClientInfo
{
	private:
		ServerInfo const	&_server;
		std::string			_request;
		bool				_ready;
		std::string			_response;
		std::string			_method;
		std::string			_boundary;
		size_t				_idx;

	public:
		ClientInfo(ServerInfo const &server);

		void	add_request(std::string const &str);
		void	set_ready();
		void	set_method(std::string const &);
		void	set_boundary(std::string const &);
		bool	request_is_complete() const;
		void	set_response(std::string const &str);

		std::string const &get_request() const;
		ServerInfo const &get_server() const;
		bool				get_ready() const;
		std::string const &get_response() const;
};