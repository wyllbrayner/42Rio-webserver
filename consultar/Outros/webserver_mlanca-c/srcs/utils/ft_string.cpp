#include "utils/ft_string.hpp"

#include <sstream>

namespace ft {

std::vector<std::string> string::split(std::string const& str,
                                       std::string const& delimiter) {
    std::vector<std::string> substrings;
    std::string              tmp = str;

    char* word = strtok(const_cast<char*>(tmp.c_str()), delimiter.c_str());
    while (word) {
        substrings.push_back(word);
        word = strtok(NULL, delimiter.c_str());
    }
    return (substrings);
}

std::string string::trim(std::string const& str) {
    std::string result = str;

    size_t start = result.find_first_not_of(" \t\n\r");
    if (start != std::string::npos) { result.erase(0, start); }
    else {
        result.clear();
        return (result);
    }

    size_t end = result.find_last_not_of(" \t\n\r");
    if (end != std::string::npos) { result.erase(end + 1); }

    return result;
}

std::string string::removeTrailingComma(std::string const& str) {
    std::string result = str;
    if (str.empty()) { return (result); }
    if (result[result.size() - 1] == ',' || result[result.size() - 1] == ';') {
        result.erase(result.size() - 1);
    }
    return (result);
}

bool string::isnumeric(std::string const& str) {
    for (size_t i = (str[0] == '-') ? 1 : 0; i < str.size(); ++i) {
        if (!isdigit(str[i])) { return (false); }
    }
    return (true);
}

int string::stoi(std::string const& str) {
    std::stringstream ss(str);
    int               result;
    ss >> result;
    return (result);
}

unsigned long string::stoul(std::string const& str) {
    std::stringstream ss(str);
    unsigned long     result;
    ss >> result;
    return (result);
}

} // namespace ft