#include "../../includes/webserv.hpp"

void	HTTPRequest::set_upload_files(std::string const &request)
{
	std::string line;
	std::stringstream ss(request);
	
	UploadFile file;
	while (getline(ss, line, '\n'))
	{
		// std::cout << line <<"\n";
		if (line.size() > _boundary.size() + 2 && line.substr(0, _boundary.size() + 2) == "--" + _boundary)
		{
			if (file.get_file_name() != "")
				_upload_files.push_back(file);

			file = UploadFile();
			
			if (!getline(ss, line, '\n'))
				break ;

			size_t idx = line.find("filename=");
			if (idx == std::string::npos)
				continue ;
			std::string filename;
			for (size_t i = idx + 10 ; i < line.size() && line[i] != '\"' ; ++i)
				filename.push_back(line[i]);
			
			file.set_file_name(filename);

			if (!getline(ss, line, '\n'))
				break ;
			if (!getline(ss, line, '\n'))
				break ;
		}
		else
			file.add_content(line);
	}
}

void	HTTPRequest::set_query_string(std::string const &request)
{
	size_t last_new_line = request.find_last_of("\n");
	if (last_new_line == std::string::npos || _query_string != "")
		throw(BadRequest);
	_query_string = request.substr(last_new_line + 1);
}

HTTPRequest::HTTPRequest(ServerInfo const &server, std::string const &request):_behavior(none),_file_uri(""), _second_file_uri(""), _query_string(""), _content_length(""), _content_type(""), _boundary(""), _accept(""), _location(NULL) // throw status_code if error
{
	std::stringstream	ss(request);

	if (!(ss >> _method))
		throw(BadRequest);
	if (!(ss >> _path))
		throw(BadRequest);
	if (_method == "GET" && _path.size() > GET_REQUEST_URI_LIMIT)
		throw(URITooLong);
	if (!(ss >> _HTTP_version))
		throw(BadRequest);
	
	if (_method != "POST" && _method != "GET" && _method != "DELETE")
		throw(MethodNotAllowed);
	
	size_t path_end_idx = _path.find('?');
	if (path_end_idx != std::string::npos)
	{
		_query_string = _path.substr(path_end_idx + 1);
		_path = _path.substr(0, path_end_idx);
	}

	if (_method == "POST")
	{
		std::string tmp;
		while (ss >> tmp)
		{
			if (tmp == "Content-Length:")
			{
				if (_content_length == "")
					ss >> _content_length;
				else
					throw(BadRequest);
			}
			else if (tmp == "Content-Type:")
			{
				if (_content_type == "")
				{
					ss >> _content_type;
					if (_content_type[_content_type.size() - 1] == ';')
						_content_type.erase(_content_type.size() - 1);
					ss >> tmp;
					if (tmp.substr(0, 9) == "boundary=")
						_boundary = tmp.substr(9);
				}
				else
					throw(BadRequest);
			}
			else if (tmp == "Accept:")
			{
				if (_accept == "")
					ss >> _accept;
				else
					throw(BadRequest);
			}
			else if (_boundary != "" && (tmp == "--" + _boundary || tmp == "--" + _boundary + "--" ))
				break ;
		}

		if (_content_length == "" || !is_number(_content_length))
			throw(LengthRequired);
		
		
		if (bigger_than(_content_length, ft::to_string(static_cast<long long>(server.get_client_body_size()) * static_cast<long long>(MB))))
			throw(PayloadTooLarge);

		if (_content_type == "")
			throw(BadRequest);

		if (_boundary != "")
			set_upload_files(request);
		else
			set_query_string(request);
	}

}

static bool find_file(HTTPRequest &request, Location const &location, std::string path, std::string const &method)
{
	std::string uri = location.get_uri();
	
	if (path.size() < uri.size() || path.substr(0, uri.size()) != uri ||
	(path.size() > uri.size() && path[uri.size()] != '/' && path[uri.size() - 1] != '/') )
		return false;

	if (location.get_redirect() != "")
	{
		request.set_file_uri(path.replace(0, uri.size(), location.get_redirect()));
		request.set_behavior(redirect);
		return true;
	}

	if (method == "POST")
	{
		if (Cgi::is_cgi(location.get_cgi(), path))
		{
			// cgi
			std::string root = location.get_root();
			if (uri[uri.size() - 1] == '/' && root[root.size() - 1] != '/')
				root += "/";
			path.replace(0, uri.size(), root);
			if (!uri_exist(path) || directory_exist(path))
				return false;
			request.set_file_uri(path);
			request.set_behavior(cgi);
			return true;
		}
		std::string upload_path = location.get_upload_path();
		if (upload_path == "" || !directory_exist(upload_path))
			return false;
		if (uri[uri.size() - 1] == '/' && upload_path[upload_path.size() - 1] != '/')
			upload_path += "/";
		path.replace(0, uri.size(), upload_path);
		bool get_method_is_allowed = false;
		std::vector<std::string> const &allow_method = location.get_allow_method();
		for (size_t i = 0 ; i < allow_method.size() ; ++i)
		{
			if (allow_method[i] == "GET")
			{
				get_method_is_allowed = true;
				break ;
			}
		}
		
		if (!get_method_is_allowed)
			request.set_behavior(post_no_get);
		else
		{
			if (location.get_autoindex())
			{
				request.set_second_file_uri(location.get_root());
				request.set_behavior(post_autoindex);
			}
			else
				request.set_behavior(post_existed_file);
			std::string index_path = location.get_index();
			if (index_path != "")
				request.set_second_file_uri(location.get_root() + "/" + index_path);
		}
		request.set_file_uri(path);
		return true;
	}

	std::string	root = location.get_root();
	if (root == "" || !directory_exist(root))
		return false;


	if (path.size() <= uri.size() + 1 && method == "GET")
	{
		if (location.get_autoindex())
		{
			request.set_file_uri(root);
			request.set_behavior(autoindex);
			return true;
		}
		std::string index_path = location.get_index();
		if (index_path != "")
		{
			if (root[root.size() - 1] != '/' && index_path[0] != '/')
				index_path = root + "/" + index_path;
			else
				index_path = root + index_path;
			if (uri_exist(index_path))
			{
				request.set_file_uri(index_path);
				request.set_behavior(existed_file);
				return true;
			}
		}
	}
	
	if (method == "GET" || method == "DELETE")
	{
		if (uri[uri.size() - 1] == '/' && root[root.size() - 1] != '/')
		root += "/";
		path.replace(0, uri.size(), root);
		if (uri_exist(path))
		{
			request.set_file_uri(path);
			request.set_behavior(existed_file);
			return true;
		}
	}

	return false;
}

void	HTTPRequest::check_request(ServerInfo const &server)
{
	// check version
	if (_HTTP_version != "HTTP/1.0" && _HTTP_version != "HTTP/1.1")
		throw(HTTPVersionNotSupported);
	
	// check path
	if (_method != "GET" && _method != "POST" && _method != "DELETE")
		throw(MethodNotAllowed);
		
	std::vector<Location>::const_iterator it;

	for (it = server.get_location().begin() ; it != server.get_location().end() ; ++it)
		if (find_file(*this, *it, _path, _method))
		{
			_location = &(*it);
			break ;
		}

	if (it == server.get_location().end())
	{
		throw(NotFound);
	}
	
	std::vector<std::string> const &allow_method = it->get_allow_method();
	for (size_t i = 0 ; i < allow_method.size() ; ++i)
		if (_method == allow_method[i])
			return ;
	
	throw(MethodNotAllowed);
}

void	HTTPRequest::set_behavior(Location_behavior behavior)
{
	_behavior = behavior;
}

void	HTTPRequest::set_file_uri(std::string const &file_uri)
{
	_file_uri = file_uri;
}

void	HTTPRequest::set_second_file_uri(std::string const &file_uri)
{
	_second_file_uri = file_uri;
}

Location_behavior	HTTPRequest::get_behavior() const
{
	return _behavior;
}

std::string const &HTTPRequest::get_method() const
{
	return _method;
}

std::string const &HTTPRequest::get_file_uri() const
{
	return _file_uri;
}


std::string const &HTTPRequest::get_second_file_uri() const
{
	return _second_file_uri;
}

std::string const &HTTPRequest::get_HTTP_versoin() const
{
	return _HTTP_version;
}

std::string const &HTTPRequest::get_content_length() const
{
	return _content_length;
}

std::string const &HTTPRequest::get_content_type() const
{
	return _content_type;
}

std::string const &HTTPRequest::get_boundary() const
{
	return _boundary;
}

std::string const &HTTPRequest::get_accept() const
{
	return _accept;
}

std::vector<UploadFile> const &HTTPRequest::get_upload_files() const
{
	return _upload_files;
}

std::string const &HTTPRequest::get_path() const
{
	return _path;
}

Location const &HTTPRequest::get_location() const
{
	if (_location == NULL)
		throw(NotFound);
	return *_location;
}

std::string const &HTTPRequest::get_query_string() const
{
	return _query_string;
}

void	HTTPRequest::print() const
{
	std::cerr	<< "HTTP Request attributes :\n"
				<< "file_uri : " << get_file_uri() <<"\n"
				<< "behavior : " << get_behavior() << "\n"
				<< "method : " << get_method() << "\n"
				<< "HTTP version : " << get_HTTP_versoin() <<"\n"
				<< "Content Length : " << get_content_length() <<"\n"
				<< "Content Type : " << get_content_type() <<"\n"
				<< "Boundary : " << get_boundary() <<"\n"
				<< "Accept : " << get_accept() <<"\n"
				<< "Content : ";
				for (size_t i = 0 ; i < get_upload_files().size() ; ++i)
				{
					std::cerr << i <<" :\n" << get_upload_files()[i].get_content() <<"\n";
				}
				std::cerr <<"\n";
}
