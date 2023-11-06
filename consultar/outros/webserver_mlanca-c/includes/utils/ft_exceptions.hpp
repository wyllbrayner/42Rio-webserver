#ifndef FT_EXCEPTIONS_HPP
#define FT_EXCEPTIONS_HPP

#include <exception>

namespace ft {

struct InvalidFileException : public std::exception {
        char const* what() const throw();
};

struct FailedToOpenFileException : public std::exception {
        char const* what(void) const throw();
};

} // namespace ft

#endif /* FT_EXCEPTIONS_HPP */