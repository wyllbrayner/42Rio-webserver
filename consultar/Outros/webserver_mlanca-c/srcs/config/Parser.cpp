#include "config/Parser.hpp"

#include "config/DirectiveTypeTraits.hpp"
#include "config/Line.hpp"
#include "config/Options.hpp"
#include "utils/ft_exceptions.hpp"
#include "utils/Logger.hpp"

#include <iostream>

namespace config {

Parser::Parser(ft::file filename) : m_line(), m_pos(0) {

    // validating file
    if (!filename.isValid()) { throw ft::InvalidFileException(); }
    // opening file
    m_file.open(std::string(filename).c_str());
    if (!m_file.is_open()) { throw ft::FailedToOpenFileException(); }
}

Parser::~Parser(void) { m_file.close(); }

bool Parser::nextLine(void) {
    if (m_file.eof()) { return (false); }
    std::getline(m_file, m_line);
    m_pos++;
    return (true);
}

std::string Parser::getLine(void) const { return (m_line); }

size_t Parser::getPosition(void) const { return (m_pos); }

std::vector<ServerOpts> const& Parser::getOptions(void) const {
    return (m_directives);
}

void Parser::error(void) const {
    LOG_E("config: syntax error: " << m_line << " (:" << m_pos << ")");
    throw InvalidSyntaxException();
}

template<LineType T>
void Parser::parseLine(std::vector<std::string> const& args, int level) {

    // creating the directive based on line type
    DirectiveTypeTraits<T> directive(args);
    // checking if directive is valid
    if (!directive.isValid()) { error(); }

    // checking if the directive's level (position) is valid
    if (level == 0) {
        // checking if directive is valid outside a server block
        if (!directive.isGlobalDirective()) {
            LOG_W("directive " << directive.getName()
                               << " is not a global directive.");
            error();
        }
        // updating m_directives
        directive.set(m_directives);
    }
    if (level == 1) {
        // checking if directive is valid inside server block
        if (!directive.isBlockDirective()) {
            LOG_W("directive " << directive.getName()
                               << " is not a server directive.");
            error();
        }
        // updating m_directives
        directive.set(m_directives.back());
    }
    else if (level == 2) {
        // checking if directive is valid inside location block
        if (!directive.isRouteDirective()) {
            LOG_W("directive " << directive.getName()
                               << " is not a location directive.");
            error();
        }
        // updating m_directives
        directive.set(m_directives.back().m_locations.back());
    }
}

template<>
void Parser::parseLine<LineEnd>(std::vector<std::string> const& args,
                                int                             level) {
    // checking if the directive's level (position) is valid
    if (level == 0) {
        LOG_W("directive { is not a global directive.");
        error();
    }
    if (level == 1) {
        // creating the directive based on line type
        DirectiveTypeTraits<LineEnd> directive(args, m_directives);

        // checking if directive is valid
        if (!directive.isValid()) { error(); }
    }
    if (level == 2) {
        // creating the directive based on line type
        DirectiveTypeTraits<LineEnd> directive(
            args, m_directives.back().m_locations.back());

        // checking if directive is valid
        if (!directive.isValid()) { error(); }
    }
}

char const* Parser::InvalidSyntaxException::what(void) const throw() {
    return ("config::parser: invalid syntax.");
}

void Parser::parse(ft::file const& filename) {
    // creating a config parser for filename
    Parser parser(filename);

    // parsing file per line
    int level = 0;
    while (parser.nextLine()) {
        // creating a line object
        Line line(parser.getLine());

        // checking line type
        switch (line.getType()) {
            case (LineEmpty): break;

            case (LineBlock):
                parser.parseLine<LineBlock>(line.format(), level);
                level++;
                break;

            case (LineRoute):
                parser.parseLine<LineRoute>(line.format(), level);
                level++;
                break;

            case (LineEnd):
                parser.parseLine<LineEnd>(line.format(), level);
                level--;
                break;

            case (LineListen):
                parser.parseLine<LineListen>(line.format(), level);
                break;

            case (LineServerName):
                parser.parseLine<LineServerName>(line.format(), level);
                break;

            case (LineRoot):
                parser.parseLine<LineRoot>(line.format(), level);
                break;

            case (LineErrorPage):
                parser.parseLine<LineErrorPage>(line.format(), level);
                break;

            case (LineMaxBodySize):
                parser.parseLine<LineMaxBodySize>(line.format(), level);
                break;

            case (LineAllowMethods):
                parser.parseLine<LineAllowMethods>(line.format(), level);
                break;

            case (LineIndex):
                parser.parseLine<LineIndex>(line.format(), level);
                break;

            case (LineAutoIndex):
                parser.parseLine<LineAutoIndex>(line.format(), level);
                break;

            case (LineCgiExtension):
                parser.parseLine<LineCgiExtension>(line.format(), level);
                break;

            case (LineUnknown):
            default: parser.error();
        }
    }
    if (level != 0) { 
        LOG_W("Failed to find end of block");
        parser.error(); }
    // initializing options
    Options::getInstance(parser.getOptions());
}

} // namespace config
