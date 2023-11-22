#include "Client.hpp"

Client::Client(const Request &request, Response &response) : 
    _request(request), 
    _response(response),
    _statusCode(0)
{
    this->getMethod();
}

Client::~Client()
{
}

std::string Client::getMethod(void)
{
    std::string pagePath("./static_pages/index.html");

    // I need to receive information from the header
    std::ifstream file(pagePath.c_str());
    if (!file)
    {
        this->_response.httpMessage.append("HTTP/1.1 404 Not Found\r\n\r\n");
        return "";
    }

    // Here execute	methods or CGI
    std::ostringstream page;
    page << file.rdbuf();
    file.close();

    /* std::ostringstream resp;
    resp << "HTTP/1.1 200 OK\n";
    resp << "Content-Type: text/html\n";
    resp << "Content-Length: " << "19" << "\n";
    resp << "<h1>webserver</h1>\n"; */
    page.flush();
    std::string text;
    int lenPage = page.str().size();
    text.append("HTTP/1.1 200 OK\r\n");
    text.append("Content-Type: text/html\r\n");
    std::string content_len = "Content-Length: " + lenPage;
    content_len += "\r\n\n";
    text.append(content_len);
    text.append(page.str());
    /* text.append("<html>\n");
    text.append("<body>\n");
    text.append("<h1>Hello, World!</h1>\n");
    text.append("</body>\n");
    text.append("</html>\n" );*/
    std::cout << "Response client: \n" << text << "\n\n";
    this->_response.httpMessage = text;
    _statusCode = 200;
    return (this->_response.httpMessage);
}

int Client::getStatusCode(void){
    return this->_statusCode;
}

std::string Client::readFile(std::string name){
    
    std::ifstream file(name.c_str());

    if (file.is_open()){
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string contents = buffer.str();

        std::cout << contents << "\n";
        file.close();
        _statusCode = 200;
        return contents;
    } else {
        _statusCode = 404;
        return "404 Not Found";
    }
}
