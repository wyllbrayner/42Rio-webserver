#include "webserv.hpp"

UploadFile::UploadFile():_file_name(""),_content("")
{}

UploadFile::UploadFile(UploadFile const &copy):_file_name(copy._file_name),_content(copy._content)
{}

UploadFile &UploadFile::operator=(UploadFile const &copy)
{
	if (this == &copy)
		return *this;
	_file_name = copy._file_name;
	_content = copy._content;
	return *this;
}

void	UploadFile::set_file_name(std::string const &file_name)
{
	_file_name = file_name;
}

void	UploadFile::add_content(std::string const &new_line)
{
	if (_content != "")
		_content += "\n";
	_content += new_line;
}

std::string const &UploadFile::get_file_name() const {return _file_name;}
std::string const &UploadFile::get_content() const {return _content;}
