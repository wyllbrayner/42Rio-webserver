#ifndef CONFIG_LINE_HPP
#define CONFIG_LINE_HPP

#include <string>
#include <vector>

namespace config {

#define LINE_COMMENT "#"

#define LINE_SERVER   "server"
#define LINE_LOCATION "location"
#define LINE_END      "}"

#define LINE_LISTEN        "listen"
#define LINE_SERVER_NAME   "server_name"
#define LINE_ROOT          "root"
#define LINE_ERROR_PAGE    "error_page"
#define LINE_MAX_BODY_SIZE "client_max_body_size"
#define LINE_ALLOW_METHODS "allow_methods"
#define LINE_INDEX         "index"
#define LINE_AUTOINDEX     "autoindex"
#define LINE_FASTCGI       "fastcgi_pass"

typedef enum {
    LineEmpty,
    LineBlock,
    LineRoute,
    LineEnd,
    LineListen,
    LineServerName,
    LineRoot,
    LineErrorPage,
    LineMaxBodySize,
    LineAllowMethods,
    LineIndex,
    LineAutoIndex,
    LineCgiExtension,
    LineUnknown
} LineType;

class Line {
    public:

        Line(void);
        Line(Line const& src);
        Line(std::string const& line);

        ~Line(void);

        Line& operator=(Line const& rhs);

        std::string getLine(void) const;
        LineType    getType(void) const;

        std::vector<std::string> format(void) const;

    private:

        std::string m_line;
        LineType    m_type;

        static LineType convertLineType(std::string const& line);
};

} // namespace config

#endif /* CONFIG_LINE_HPP */