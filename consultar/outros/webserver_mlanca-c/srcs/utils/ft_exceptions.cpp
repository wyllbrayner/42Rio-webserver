#include "utils/ft_exceptions.hpp"

namespace ft {

char const* InvalidFileException::what(void) const throw() {
    return ("ft::exception: invalid file.");
}

char const* FailedToOpenFileException::what(void) const throw() {
    return ("ft::exception: failed to open file.");
}

} // namespace ft
