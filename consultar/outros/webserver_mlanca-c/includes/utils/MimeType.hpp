#ifndef HTTP_MIME_TYPE_HPP
#define HTTP_MIME_TYPE_HPP

#include "utils/ft_filesystem.hpp"

#include <string>

namespace http {

class MimeType {
    public:

        MimeType(void);
        MimeType(ft::file const& file);

        operator std::string(void) const;

        bool operator==(MimeType const& rhs) const;

    private:

        std::string getMimeType(void);

        std::string m_ext;
        std::string m_type;
};

} // namespace http

#endif /* HTTP_MIME_TYPE_HPP */