#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include "utils/ft_exceptions.hpp"
#include "utils/ft_string.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <sys/stat.h>

namespace ft {

class directory {
    public:

        directory(void);
        directory(char const* s);
        directory(std::string const& s);

        directory& operator=(std::string const& str);
        directory& operator=(char const* str);

        operator std::string(void) const;

        bool operator==(directory const& rhs) const;

        bool exists(void) const;
        bool isValid(void) const;
        bool isCrawler(void) const;
        bool isFile(void) const;

    private:

        std::string addTrailingSlash(std::string const& s) const;
        std::string removeTrailingSlash(std::string const& s) const;

        std::string m_dir;
};

std::ostream& operator<<(std::ostream& os, directory const& rhs);

class file {
    public:

        file(void);
        file(char const* s);
        file(std::string const& s);

        file& operator=(std::string const& str);
        file& operator=(char const* str);

        operator std::string(void) const;

        bool operator==(file const& rhs) const;

        bool exists(void) const;
        bool isValid(void) const;
        bool isDirectory(void) const;
        bool isCrawler(void) const;

        std::string read(void) const;
        std::string write(std::string const& str) const;

        std::string getExtension(void) const;
        directory   getDirectory(void) const;
        std::string getFilename(void) const;

    private:

        std::string m_file;
};

std::ostream& operator<<(std::ostream& os, file const& rhs);

} // namespace ft

#endif /* FILESYSTEM_HPP */