#include "Client.hpp"

Client::Client(const Request &request, Response & response) :
    _request(request), _response(response)
{ 
    this->getMethod();
}

Client::~Client()
{ }

std::string Client::getMethod(void) {
    std::string pagePath("./static_pages/index.html");

    //I need to receive information from the header
    std::ifstream file(pagePath.c_str());
    if (!file) {
        this->_response.httpMessage.append("HTTP/1.1 404 Not Found\r\n\r\n");
        return "";
    }

	// Here execute	methods or CGI
    std::ostringstream page;
    page << file.rdbuf();
    file.close();

    std::ostringstream resp;
    resp << "HTTP/1.1 200 OK\n";
    resp << "Content-Type: text/html; charset=UTF-8\n";
    resp << "Content-Length: "  << page.str().length()  << " \n";
    resp << page.str();
    
    this->_response.httpMessage = resp.str();
    _statusCode = 200;
    return (this->_response.httpMessage);
}