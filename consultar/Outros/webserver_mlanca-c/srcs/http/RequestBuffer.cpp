#include "http/RequestBuffer.hpp"

#include "utils/ft_string.hpp"
#include "utils/Logger.hpp"

namespace http {

RequestBuffer::RequestBuffer(void) {}

RequestBuffer::RequestBuffer(RequestBuffer const& src) { *this = src; }

RequestBuffer::~RequestBuffer(void) {}

RequestBuffer& RequestBuffer::operator=(RequestBuffer const& rhs) {
    if (this != &rhs) { m_buffers = rhs.m_buffers; }
    return (*this);
}

RequestBuffer& RequestBuffer::getInstance(void) {
    static RequestBuffer ist;
    return (ist);
}

std::string RequestBuffer::getNextRequest(int fd, std::string reqStr) {
    std::string req;
    std::string buff = reqStr;

    RequestBuffer& rb = getInstance();
    // trying to find if fd is already in buffer list
    if (rb.m_buffers.find(fd) != rb.m_buffers.end()) {
        // updating buff with previous buffer
        buff = rb.m_buffers[fd];
        buff += reqStr;
        if (buff.empty()) {
            // if buffer is empty, then the fd is removed from buffers list
            rb.m_buffers.erase(fd);
            return (req);
        }
    }
    else {
        // adding buffer to list
        rb.m_buffers.insert(std::make_pair(fd, buff));
    }

    // parsing buff into a request

    size_t endHeaders = buff.find("\r\n\r\n");
    if (endHeaders == std::string::npos) {
        // Request is incomplete
        return (req);
    }

    // getting request until the end of headers
    req = buff.substr(0, endHeaders + 4);

    // parsing body of request - if there is one
    size_t startPos = req.find("Content-Length: ");
    if (startPos != std::string::npos) {
        size_t endPos = req.find("\r\n", startPos);
        // getting Content-Length
        int len = ft::string::stoi(buff.substr(startPos + 16, endPos));

        // checking if body is complete
        if (buff.size() < endHeaders + 4 + len) {
            req.clear();
            return (req);
        }

        // adding body to ret
        if (len) { req += buff.substr(endHeaders + 4, len); }
    }

    // removing req from buffer
    buff = buff.substr(req.size());

    // updating buffer for this fd
    rb.m_buffers[fd] = buff;

    return (req);
}

bool RequestBuffer::hasRequest(int fd) {
    // returns true if the buffer for that fd is not empty
    return (getInstance().m_buffers.find(fd) != getInstance().m_buffers.end());
}

void RequestBuffer::cleanBuffer(int fd) {
    // clears buffer from fd
    if (getInstance().m_buffers.find(fd) != getInstance().m_buffers.end()) {
        getInstance().m_buffers.erase(fd);
    }
}

} // namespace http
