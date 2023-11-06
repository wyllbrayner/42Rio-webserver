#ifndef HTTP_ERROR_CODE_HPP
#define HTTP_ERROR_CODE_HPP

#include <string>

namespace http {

class ErrorCode {
    public:

        ErrorCode(void);
        ErrorCode(ErrorCode const& src);
        ErrorCode(int code);
        ~ErrorCode(void);

        ErrorCode& operator=(ErrorCode const& str);

        operator std::string(void) const;
        operator int(void) const;

        ErrorCode& operator=(unsigned int const& rhs);
        ErrorCode& operator=(std::string const& rhs);
        ErrorCode& operator=(char const* rhs);

        struct UnrecognizedErrorCode : public std::exception {
                char const* what(void) const throw();
        };

    private:

        static std::string const getReason(int code);

        int         m_code;
        std::string m_reason;
};

} // namespace http

#endif /* HTTP_ERROR_CODE_HPP */