#include "./Response.hpp"
#include <fstream>
#include <iostream>

Response::Response() {}

Response::~Response() {}

std::string Response::toString(int number) {
    std::stringstream ss;
    ss << number;
    std::string str = ss.str();
    return str;
}

void Response::setBody(std::string body) {
    this->_body = body;
}

void Response::createHTTPHeader(std::string statusCode, std::string contentType) {
    std::string header;

    header.append("HTTP/1.1 ").append(statusCode).append("\r\n");
    header.append("Content-Type: ").append(contentType).append("\r\n");
    header.append("Content-Length: ").append(toString(this->_body.size()));
    header.append("\r\n").append("\r\n");

    this->_header = header;
//    std::cout << "createHTTPHeader: this->_header: " << this->_header << std::endl;
}

void Response::send() {
   this->httpMessage = this->_header;
   this->httpMessage.append(this->_body);
}

void Response::processFileForHTTPResponse(std::stringstream &file, std::string statusCode)
{
    std::string content = file.str();

    this->_body = content;
    this->createHTTPHeader(statusCode, "text/html");
}