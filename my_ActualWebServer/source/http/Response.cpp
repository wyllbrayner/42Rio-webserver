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
void Response::createHTTPHeader2(std::string statusCode, std::string contentType) {
    std::string header;

    header.append("HTTP/1.1 ").append(statusCode).append("\r\n");
    header.append("Content-Type: ").append(contentType).append("\r\n");
    header.append("Content-Length: ").append(toString(this->_body.size()));
    header.append("\r\n").append("\r\n");

    this->_header = header;
//    std::cout << "createHTTPHeader2: this->_header: " << this->_header << std::endl;
}

void Response::createHTTPHeader(int statusCode, std::string contentType, int contentLength) {
    std::string header;

    this->_statusCode = statusCode;
    header.append("HTTP/1.1 ");
    header.append(toString(statusCode));
    header.append(" ");
    header.append("OK");
    header.append("\r\n");
    header.append("Content-Type: ");
    header.append(contentType);
    header.append("\r\n");
    header.append("Content-Length: ");
    header.append(toString(contentLength));
    header.append("\r\n");
    header.append("\r\n");

    this->_header = header;
//    std::cout << "createHTTPHeader: this->_header: " << this->_header << std::endl;
}

void Response::send() {
   this->httpMessage = this->_header;
   this->httpMessage.append(this->_body);
}

//void Response::processFileForHTTPResponse2(std::string &file, std::string statusCode)
void Response::processFileForHTTPResponse2(std::stringstream &file, std::string statusCode)
{
//    std::ifstream fileStream(file.c_str());
    
//    std::stringstream buffer;
//    buffer << fileStream.rdbuf();
    std::string content = file.str();

    this->_body = content;
    this->createHTTPHeader2(statusCode, "text/html");
}
void Response::processFileForHTTPResponse(std::string &file, int statusCode)
{
    std::ifstream fileStream(file.c_str());
    
    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    std::string content = buffer.str();

    this->_body = content;
    this->createHTTPHeader(statusCode, "text/html", this->_body.size());
}