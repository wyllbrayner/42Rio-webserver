#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "config/DirectiveTypeTraits.hpp"
#include "config/ServerOpts.hpp"
#include "utils/ft_filesystem.hpp"

#include <typeinfo>
#include <vector>

namespace config {

class Parser {
    public:

        Parser(ft::file filename);
        ~Parser(void);

        bool nextLine(void);

        std::string getLine(void) const;
        size_t      getPosition(void) const;

        void error(void) const;

        template<LineType T>
        void parseLine(std::vector<std::string> const& args, int level);

        struct InvalidSyntaxException : public std::exception {
                char const* what(void) const throw();
        };

        std::vector<ServerOpts> const& getOptions(void) const;

        static void parse(ft::file const& filename);

    private:

        std::string   m_line;
        size_t        m_pos;
        std::ifstream m_file;

        std::vector<ServerOpts> m_directives;
};

template<>
void Parser::parseLine<LineEnd>(std::vector<std::string> const& args, int level);

} // namespace config

#endif /* CONFIG_PARSER_HPP */