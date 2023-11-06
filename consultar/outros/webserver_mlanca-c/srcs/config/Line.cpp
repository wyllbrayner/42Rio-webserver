#include "config/Line.hpp"

#include "utils/ft_string.hpp"

namespace config {

Line::Line(Line const& src) : m_line(src.m_line), m_type(src.m_type) {}

Line::Line(std::string const& line) : m_line(ft::string::trim(line)) {
    m_type = Line::convertLineType(m_line);
}

Line::~Line(void) {}

Line& Line::operator=(Line const& rhs) {
    if (this != &rhs) {
        m_line = rhs.m_line;
        m_type = rhs.m_type;
    }
    return (*this);
}

std::string Line::getLine(void) const { return (m_line); }

LineType Line::getType(void) const { return (m_type); }

LineType Line::convertLineType(std::string const& line) {
    std::vector<std::string> subs = ft::string::split(line, " \t");
    if (subs.empty() || subs[0] == LINE_COMMENT) { return (LineEmpty); }
    if (subs[0] == LINE_SERVER) { return (LineBlock); }
    if (subs[0] == LINE_LOCATION) { return (LineRoute); }
    if (subs[0] == LINE_END) { return (LineEnd); }
    if (subs[0] == LINE_LISTEN) { return (LineListen); }
    if (subs[0] == LINE_SERVER_NAME) { return (LineServerName); }
    if (subs[0] == LINE_ROOT) { return (LineRoot); }
    if (subs[0] == LINE_ERROR_PAGE) { return (LineErrorPage); }
    if (subs[0] == LINE_MAX_BODY_SIZE) { return (LineMaxBodySize); }
    if (subs[0] == LINE_ALLOW_METHODS) { return (LineAllowMethods); }
    if (subs[0] == LINE_INDEX) { return (LineIndex); }
    if (subs[0] == LINE_AUTOINDEX) { return (LineAutoIndex); }
    if (subs[0] == LINE_FASTCGI) { return (LineCgiExtension); }
    return (LineUnknown);
}

std::vector<std::string> Line::format(void) const {
    std::vector<std::string> subs;
    if (m_type == LineEmpty || m_type == LineUnknown) { return (subs); }

    size_t      pos = m_line.find('#');
    std::string str =
        (pos == std::string::npos) ? m_line : m_line.substr(0, pos);
    str = ft::string::trim(str);

    if ((m_type == LineBlock || m_type == LineRoute) &&
        str.find('{') != str.size() - 1) {
        return (subs);
    }
    else if (m_type == LineEnd && str != "}") { return (subs); }
    else if (m_type > LineEnd && str.find(';') != str.size() - 1) {
        return (subs);
    }

    str = (str.find(';') != std::string::npos) ? (str.substr(0, str.size() - 1))
                                               : str;

    subs = ft::string::split(str, " \t");
    return (subs);
}

} // namespace config
