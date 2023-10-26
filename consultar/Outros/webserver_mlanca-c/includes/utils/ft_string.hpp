#ifndef FT_STRING_HPP
#define FT_STRING_HPP

#include <cstring>
#include <string>
#include <vector>

namespace ft {

class string {
    public:

        static std::vector<std::string> split(std::string const& str,
                                              std::string const& delimiter);

        static std::string trim(std::string const& str);
        static std::string removeTrailingComma(std::string const& str);

        static bool          isnumeric(std::string const& str);
        static int           stoi(std::string const& str);
        static unsigned long stoul(std::string const& str);
};

} // namespace ft

#endif /* FT_STRING_HPP */