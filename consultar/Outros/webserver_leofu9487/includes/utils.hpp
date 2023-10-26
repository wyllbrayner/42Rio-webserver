#pragma once
#include "webserv.hpp"

void	print_log(std::string const &msg);
void	make_socket_nonblock(int fd);
bool	directory_exist(std::string const &dir);
bool	uri_exist(std::string const &file);
bool	file_exist(std::string const &file);
bool	bigger_than(std::string const &a, std::string const &b);
bool	is_number(std::string const &);
int		string_to_int(std::string const &);
bool	is_executable(std::string const &);
bool	is_readable(std::string const &);
std::string get_file_type(std::string const &file);
std::string get_file_content(std::string const &content);
std::string file_or_directory(std::string const &uri);
void	change_file_name(std::string &);
void	delete_file(std::string const &);

namespace  ft
{
    std::string to_string(int n);
	std::string to_string(size_t n);
	std::string to_string(long long n);
}
