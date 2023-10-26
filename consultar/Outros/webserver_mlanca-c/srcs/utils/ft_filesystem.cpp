#include "utils/ft_filesystem.hpp"

#include "utils/Logger.hpp"

namespace ft {

/* directory */
directory::directory(void) {}

directory::directory(char const* s) : m_dir(addTrailingSlash(s)) {}

directory::directory(std::string const& s) : m_dir(addTrailingSlash(s)) {}

directory& directory::operator=(std::string const& str) {
    m_dir = addTrailingSlash(str);
    return (*this);
}

directory& directory::operator=(char const* str) {
    m_dir = addTrailingSlash(str);
    return (*this);
}

bool directory::operator==(directory const& rhs) const {
    return (m_dir == rhs.m_dir);
}

directory::operator std::string(void) const { return (m_dir); }

bool directory::exists(void) const {
    struct stat s;
    if (stat(m_dir.c_str(), &s) == 0) {
        if (s.st_mode & S_IFDIR) { return (true); }
    }
    return (false);
}

bool directory::isValid(void) const {
    if (!exists() || isCrawler()) { return (false); }
    return (true);
}

bool directory::isFile(void) const {

    std::string tmp = removeTrailingSlash(m_dir);
    if (!exists()) {
        struct stat s;
        if (stat(tmp.c_str(), &s) == 0) {
            if (s.st_mode & S_IFREG) { return (true); }
        }
    }
    return (false);
}

bool directory::isCrawler(void) const {
    std::vector<std::string> dirs = ft::string::split(m_dir, "/");

    int count = 0;
    for (std::vector<std::string>::iterator it = dirs.begin(); it != dirs.end();
         ++it) {
        if (*it == "..") { count--; }
        else if (*it != ".") { count++; }
        if (count < 0) { return (true); }
    }
    return (false);
}

std::string directory::addTrailingSlash(std::string const& str) const {
    std::string tmp = str;
    if (!tmp.empty() && tmp[tmp.size() - 1] != '/') { tmp.append("/"); }
    return (tmp);
}

std::string directory::removeTrailingSlash(std::string const& str) const {
    std::string tmp = str;
    if (!tmp.empty() && tmp[tmp.size() - 1] == '/') {
        tmp.assign(tmp.substr(0, tmp.size() - 1));
    }
    return (tmp);
}

std::ostream& operator<<(std::ostream& os, directory const& dir) {
    os << dir.operator std::string();
    return (os);
}

/* file */
file::file(void) {}

file::file(char const* s) : m_file(s) {}

file::file(std::string const& s) : m_file(s) {}

file& file::operator=(std::string const& str) {
    m_file = str;
    return (*this);
}

file& file::operator=(char const* str) {
    m_file = str;
    return (*this);
}

file::operator std::string(void) const { return (m_file); }

bool file::operator==(file const& rhs) const { return (m_file == rhs.m_file); }

bool file::exists(void) const {
    struct stat s;
    if (stat(m_file.c_str(), &s) == 0) {
        if (s.st_mode & S_IFREG) { return (true); }
    }
    return (false);
}

bool file::isValid(void) const {
    if (!exists() || isCrawler()) { return (false); }
    return (true);
}

bool file::isDirectory(void) const {
    struct stat s;
    if (stat(m_file.c_str(), &s) == 0) {
        if (s.st_mode & S_IFDIR) { return (true); }
    }
    return (false);
}

bool file::isCrawler(void) const {
    std::vector<std::string> dirs = ft::string::split(m_file, "/");

    int count = 0;
    for (std::vector<std::string>::iterator it = dirs.begin(); it != dirs.end();
         ++it) {
        if (*it == "..") { count--; }
        else if (*it != ".") { count++; }
        if (count < 0) { return (true); }
    }
    return (false);
}

std::string file::read(void) const {
    if (!exists() || isDirectory()) { throw InvalidFileException(); }
    std::ifstream file(m_file.c_str());
    if (!file.is_open()) { throw FailedToOpenFileException(); }

    std::stringstream ss;
    ss << file.rdbuf();
    file.close();

    return (ss.str());
}

std::string file::write(std::string const& str) const {
    if (!isValid() || isDirectory()) { throw InvalidFileException(); }
    std::ofstream file(m_file.c_str());
    if (!file.is_open()) { throw FailedToOpenFileException(); }

    file << str;
    file.close();

    return (str);
}

std::string file::getExtension(void) const {
    std::string              ext;
    std::vector<std::string> subs = ft::string::split(m_file, ".");

    if (subs.size() > 1) { ext = "." + subs[subs.size() - 1]; }
    return (ext);
}

directory file::getDirectory(void) const {
    if (isDirectory()) { return (m_file); }
    else {
        size_t      pos = m_file.find_last_of('/');
        std::string dir = m_file.substr(0, pos);
        if (dir.empty() || pos == std::string::npos) { dir = "./"; }
        return (directory(dir));
    }
}

std::string file::getFilename(void) const {
    size_t pos = m_file.find_last_of('/');
    if (pos == std::string::npos) { return (m_file); }
    return (m_file.substr(pos + 1));
}

std::ostream& operator<<(std::ostream& os, file const& file) {
    os << file.operator std::string();
    return (os);
}

} // namespace ft
