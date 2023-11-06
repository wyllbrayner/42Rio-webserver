#pragma once
#include "webserv.hpp"


class UploadFile
{
private:
	std::string _file_name;
	std::string _content;

public:
	UploadFile();
	UploadFile(UploadFile const &copy);
	UploadFile &operator=(UploadFile const &copy);

	void	set_file_name(std::string const &);
	void	add_content(std::string const &);

	std::string const &get_file_name() const;
	std::string const &get_content() const;

};