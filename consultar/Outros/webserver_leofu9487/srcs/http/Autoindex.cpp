#include "webserv.hpp"

Autoindex::Autoindex(std::string uri, std::string const &root)
{
	DIR *dir;
    struct dirent *ent;
	std::vector<std::string> files;
	if ((dir = opendir(root.c_str())) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
			if (std::string(ent->d_name) != "." && std::string(ent->d_name) != "..")
				files.push_back(std::string(ent->d_name));
        closedir(dir);
    }
    else
        throw(NotFound);

	if (uri[uri.size() - 1] != '/')
		uri += "/";
	// std::vector<std::pair<std::string, std::string> > files_and_types;
    // if ((dir = opendir(root.c_str())) != NULL)
    // {
    //     while ((ent = readdir(dir)) != NULL)
    //         files_and_types.push_back(std::make_pair(std::string(ent->d_name), file_or_directory(std::string(ent->d_name))));
    //     closedir(dir);
    // }
    // else
    //     throw(NotFound);
    
	_html += "<html>\r\n";
	_html += "<head>\r\n";
	_html += "<meta charset=\"utf-8\">\r\n";
	_html += "<title>Autoindex</title>\r\n";
	_html += "</head>\r\n";
	_html += "<body>\r\n";
	_html += "<h1> Current Direcotry : " + uri + "</h1>\r\n";
	_html += "Files : \r\n<ul>";
	for (std::vector<std::string>::iterator it = files.begin() ; it != files.end() ; ++it)
	{
		_html += "<li><a href=\"" + uri + *it + "\"> " + *it + "</a></li>\r\n";
	}
	_html += "</ul></body></html>\r\n";
}

std::string const &Autoindex::get_html() const
{
	return _html;
}		
