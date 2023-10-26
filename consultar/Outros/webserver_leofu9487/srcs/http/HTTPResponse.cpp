#include "../../includes/webserv.hpp"

std::map<int, std::string> HTTPResponse::_status_line = init_code_status();
std::map<std::string, std::string> HTTPResponse::_content_type = init_content_type();

void HTTPResponse::set_HTTP_version(std::string const &str) { _HTTP_version = str; }
void HTTPResponse::set_status_code(status_code status) { if (_status_code == Undefined) _status_code = status;}
void HTTPResponse::set_file_uri(std::string const &uri) { _file_uri = uri; }

std::string const &HTTPResponse::get_HTTP_version() const { return _HTTP_version; }
status_code HTTPResponse::get_status_code() const { return _status_code; }
std::string const &HTTPResponse::get_file_uri() const { return _file_uri; }
std::string const &HTTPResponse::get_second_file_uri() const { return _second_file_uri; }
std::string const &HTTPResponse::get_msg() const { return _msg; }

std::map<int, std::string> HTTPResponse::init_code_status()
{
			std::map<int, std::string> status;
			status[100] = "Continue";
			status[101] = "Switching Protocols";
			status[102] = "Processing";
			status[103] = "Early Hints";
			status[200] = "OK";
			status[201] = "Created";
			status[202] = "Accepted";
			status[203] = "Non-Authoritative Information";
			status[204] = "No Content";
			status[205] = "Reset Content";
			status[206] = "Partial Content";
			status[207] = "Multi-Status";
			status[208] = "Already Reported";
			status[226] = "IM Used";
			status[300] = "Multiple Choices";
			status[301] = "Moved Permanently";
			status[302] = "Found";
			status[303] = "See Other";
			status[304] = "Not Modified";
			status[305] = "Use Proxy";
			status[306] = "Switch Proxy";
			status[307] = "Temporary Redirect";
			status[308] = "Permanent Redirect";
			status[400] = "Bad Request";
			status[401] = "Unauthorized";
			status[402] = "Payment Required";
			status[403] = "Forbidden";
			status[404] = "Not Found";
			status[405] = "Method Not Allowed";
			status[406] = "Not Acceptable";
			status[407] = "Proxy Authentication Required";
			status[408] = "Request Timeout";
			status[409] = "Conflict";
			status[410] = "Gone";
			status[411] = "Length Required";
			status[412] = "Precondition Failed";
			status[413] = "Payload Too Large";
			status[414] = "URI Too Long";
			status[415] = "Unsupported Media Type";
			status[416] = "Range Not Satisfiable";
			status[417] = "Expectation Failed";
			status[418] = "I\'m a teapot"; 
			status[421] = "Misdirected Request";
			status[422] = "Unprocessable Entity";
			status[423] = "Locked";
			status[424] = "Failed Dependency";
			status[425] = "Too Early";
			status[426] = "Upgrade Required";
			status[428] = "Precondition Required";
			status[429] = "Too Many Requests";
			status[431] = "Request Header Fields Too Large";
			status[451] = "Unavailable For Legal Reasons";
			status[500] = "Internal Server Error";
			status[501] = "Not Implemented";
			status[502] = "Bad Gateway";
			status[503] = "Service Unavailable";
			status[504] = "Gateway Timeout";
			status[505] = "HTTP Version Not Supported";
			status[506] = "Variant Also Negotiates";
			status[507] = "Insufficient Storage";
			status[508] = "Loop Detected";
			status[510] = "Not Extended";
			status[511] = "Network Authentication Required";

			return status;
		}

/*
application/java-archive
application/EDI-X12   
application/EDIFACT   
application/javascript   
application/octet-stream   
application/ogg   
application/pdf  
application/xhtml+xml   
application/x-shockwave-flash    
application/json  
application/ld+json  
application/xml   
application/zip  
application/x-www-form-urlencoded  
audio/mpeg   
audio/x-ms-wma   
audio/vnd.rn-realaudio   
audio/x-wav  
image/gif   
image/jpeg   
image/png   
image/tiff    
image/vnd.microsoft.icon    
image/x-icon   
image/vnd.djvu   
image/svg+xml    
multipart/mixed    
multipart/alternative   
multipart/related (using by MHTML (HTML mail).)  
multipart/form-data  
text/css    
text/csv    
text/html    
text/javascript (obsolete)    
text/plain    
text/xml    
video/mpeg    
video/mp4    
video/quicktime    
video/x-ms-wmv    
video/x-msvideo    
video/x-flv   
video/webm   
application/vnd.android.package-archive
application/vnd.oasis.opendocument.text    
application/vnd.oasis.opendocument.spreadsheet  
application/vnd.oasis.opendocument.presentation   
application/vnd.oasis.opendocument.graphics   
application/vnd.ms-excel    
application/vnd.openxmlformats-officedocument.spreadsheetml.sheet   
application/vnd.ms-powerpoint    
application/vnd.openxmlformats-officedocument.presentationml.presentation    
application/msword   
application/vnd.openxmlformats-officedocument.wordprocessingml.document   
application/vnd.mozilla.xul+xml   
*/

std::map<std::string, std::string> HTTPResponse::init_content_type()
{
			// https://stackoverflow.com/questions/23714383/what-are-all-the-possible-values-for-http-content-type-header			
			std::map<std::string, std::string> type;
			type["jar"] = "application/java-archive";
			type["gif"] = "image/gif";
			type["jpg"] = "image/jpeg";
			type["jpeg"] = "image/jpeg";
			type["png"] = "image/png";
			type["css"] = "text/css";
			type["html"] = "text/html";
			type["htm"] = "text/html";
			type["javascript"] = "text/javascript";
			type[""] = "text/plain";
			type["txt"] = "text/plain";
			return type;
}

std::string HTTPResponse::get_default_error_page(status_code error_code)
{
	// todo
	std::string msg; // write the content of html file

	msg += ft::to_string(error_code) + " " + _status_line[error_code] + "\r\n";
	
	std::string file;
	file = "HTTP/1.1 " + ft::to_string(error_code) + " " + _status_line[error_code] + "\r\n";
	file += "Content-Type: text/html\r\nContent-Length: " + ft::to_string(msg.size()) + "\r\n\n" + msg;

	return file;
}

HTTPResponse::HTTPResponse(std::string const &HTTP_version, std::string const &method, std::string const &file_uri, std::string const &second_file_uri, std::string const &path, Location_behavior behavior, std::vector<UploadFile> const &upload_files, status_code status) : _HTTP_version(HTTP_version), _status_code(status), _file_uri(file_uri), _second_file_uri(second_file_uri)
{
	if (behavior == post_autoindex || behavior == post_existed_file || behavior == post_no_get)
	{
		// source : https://reqbin.com/Article/HttpPost#:~:text=POST%20is%20an%20HTTP%20method,and%20images%20to%20the%20server.

		for (size_t i = 0 ; i < upload_files.size() ; ++i)
		{
			std::string file_name(file_uri[file_uri.size() - 1] == '/' ? file_uri + upload_files[i].get_file_name() : file_uri + "/" + upload_files[i].get_file_name());
			if (uri_exist(file_name))
				change_file_name(file_name);
			std::ofstream out_file(file_name.c_str());
			if (!out_file)
			{
				set_status_code(UnprocessableEntity);
				break ;
			}
			out_file << upload_files[i].get_content();
			out_file.close();
		}
		set_status_code(Created);
		// _msg = HTTPResponse("1.1", "GET", file_uri, existed_file, std::vector<UploadFile>()/*, _status_code*/).get_msg();
		
		// _msg = "HTTP/" + get_HTTP_version() + " " + ft::to_string(get_status_code()) + " " + _status_line[get_status_code()] + "\r\n";
		// _msg += "Content-Type: " +  + "\r\nContent-Length:" + ft::to_string(content.size()) + "\r\n\n" + content;
	}
	
	if (behavior == post_no_get)
	{
		throw(MethodNotAllowed);
	}

	if (behavior == existed_file || behavior == post_existed_file)
	{
		if (method == "GET" || method == "POST")
		{
			std::string real_file_uri = (method == "GET" ? get_file_uri() : get_second_file_uri());
			std::map<std::string, std::string>::iterator file_type_iterator = _content_type.find(get_file_type(real_file_uri));
			if (directory_exist(real_file_uri)) // want to read a file, but it's a directory
				throw(Forbidden);
			if (!uri_exist(real_file_uri))
				throw(NotFound);
			if (file_type_iterator == _content_type.end())
				throw(UnsupportedMediaType);
			std::string const &content = get_file_content(real_file_uri);
			
			if (content.size() == 0)
				set_status_code(NoContent);
			else
				set_status_code(OK);;

			_msg = "HTTP/" + get_HTTP_version() + " " + ft::to_string(get_status_code()) + " " + _status_line[get_status_code()] + "\r\n";
			_msg += "Content-Type: " + file_type_iterator->second + "\r\nContent-Length:" + ft::to_string(content.size()) + "\r\n\n" + content;
		}
		else if (method == "DELETE")
		{
			if (directory_exist(file_uri))
				throw(Forbidden);
			if (!uri_exist(file_uri))
				throw(NotFound);
			delete_file(file_uri);
			set_status_code(NoContent);
			_msg = "HTTP/" + get_HTTP_version() + " " + ft::to_string(get_status_code()) + " " + _status_line[get_status_code()] + "\r\n";
			_msg += "Content-Type: text/html\r\nContent-Length:0\r\n\n";
		}
		else
			throw(MethodNotAllowed);
	}
	else if (behavior == redirect)
	{
		// source : https://developer.mozilla.org/en-US/docs/Web/HTTP/Redirections
		set_status_code(MovedPermanently);
		
		_msg = "HTTP/" + get_HTTP_version() + " " + ft::to_string(get_status_code()) + " " + _status_line[get_status_code()] + "\r\n";
		_msg += "Location: " + get_file_uri();
	}
	else if (behavior == autoindex || behavior == post_autoindex)
	{
		std::string real_file_uri = (method == "GET" ? get_file_uri() : get_second_file_uri());
		if (!directory_exist(real_file_uri))
			throw(NotFound);
		Autoindex file(path, real_file_uri);

		set_status_code(OK);
		_msg = "HTTP/" + get_HTTP_version() + " " + ft::to_string(get_status_code()) + " " + _status_line[get_status_code()] + "\r\n";
		_msg += "Content-Type: text/html\r\nContent-Length:" + ft::to_string(file.get_html().size()) + "\r\n\n" + file.get_html();

	}
	else
	{
		print_log("WTF???");
	}
}

void HTTPResponse::send_error_page(int fd, std::string const &error_pages_root, status_code error_code)
{
	try
	{
		HTTPResponse response("1.1", "GET", error_pages_root + "/" + ft::to_string(error_code) + ".html", "", "", existed_file, std::vector<UploadFile>(), error_code);
		write(fd, response.get_msg().c_str(), response.get_msg().size());
		// std::cerr << response.get_msg() << std::endl;
	}
	catch (status_code status)
	{
		if (status != NotFound)
		{
			print_log("WTF?? Something wrong in srcs/http/HTTPResponse.cpp : send_error_page()\n");
			exit(EXIT_FAILURE);
		}
		write(fd, get_default_error_page(error_code).c_str(), get_default_error_page(error_code).size());
		// std::cerr << get_default_error_page(error_code) << std::endl;
	}
}
