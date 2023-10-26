#include "webserv.hpp"

void	deal_with_request(std::map<int, ClientInfo>::iterator & it)
{
	try
	{
		// std::cout << it->second.get_request() << std::endl;
		HTTPRequest	request(it->second.get_server(), it->second.get_request());
		request.check_request(it->second.get_server());
		
		Location const &location = request.get_location();

		if (request.get_behavior() == cgi || Cgi::is_cgi(location.get_cgi(), request.get_file_uri()))
		{
			Cgi cgi(location, request);
			write(it->first, cgi.get_msg().c_str(), cgi.get_msg().size());
			print_log("Respond with HTTP status : " + ft::to_string(cgi.get_status_code()) + " " + HTTPResponse::_status_line[cgi.get_status_code()]);
		}
		else
		{
			HTTPResponse response(request.get_HTTP_versoin(), request.get_method(), request.get_file_uri(), request.get_second_file_uri(), request.get_path(), request.get_behavior(), request.get_upload_files());
			write(it->first, response.get_msg().c_str(), response.get_msg().size());
			print_log("Respond with HTTP status : " + ft::to_string(response.get_status_code()) + " " + HTTPResponse::_status_line[response.get_status_code()]);
		}
	}
	catch(status_code error_code)
	{
		HTTPResponse::send_error_page(it->first, it->second.get_server().get_error(), error_code);
		print_log("Respond with HTTP status : " + ft::to_string(error_code) + " " + HTTPResponse::_status_line[error_code]);
	}
}
