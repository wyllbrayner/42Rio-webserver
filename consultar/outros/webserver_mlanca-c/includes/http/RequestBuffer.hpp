#ifndef HTTP_REQUEST_BUFFER_HPP
#define HTTP_REQUEST_BUFFER_HPP

#include <map>
#include <string>

namespace http {

class RequestBuffer {

    public:

        static std::string getNextRequest(int fd, std::string reqStr = "");
        static bool        hasRequest(int fd);
        static void        cleanBuffer(int fd);

    private:

        RequestBuffer(void);
        RequestBuffer(RequestBuffer const&);

        ~RequestBuffer(void);

        RequestBuffer& operator=(RequestBuffer const&);

        static RequestBuffer& getInstance(void);

        std::map<int, std::string> m_buffers;
};

} // namespace http

#endif /* HTTP_REQUEST_BUFFER_HPP */