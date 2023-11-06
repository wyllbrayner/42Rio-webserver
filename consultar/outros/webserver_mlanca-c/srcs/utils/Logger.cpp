#include "utils/Logger.hpp"

/* Logger */
Logger::Logger(LogLevel::level_t lvl) : m_lvl(lvl), m_ffile(false) {}

Logger::~Logger() {
    if (m_ffile) { m_file.close(); }
}

Logger& Logger::getInstance(void) {
    static Logger ist;
    return (ist);
}

void Logger::setLevel(LogLevel::level_t level) { m_lvl = level; }

int Logger::getLevel(void) const { return (m_lvl); }

std::ofstream& Logger::getFile(void) {
    if (!m_ffile) { throw ft::InvalidFileException(); }
    return (m_file);
}

void Logger::setFile(std::string filename) {
    if (m_ffile) { m_file.close(); }
    m_file.open(filename.c_str());
    if (!m_file.good()) { throw ft::InvalidFileException(); }
    m_ffile = true;
}

std::string Logger::log(LogLevel::level_t lvl, std::string file, int line) {
    switch (lvl) {
        case LogLevel::Debug: return (Formatter::debug(file, line));
        case LogLevel::Info: return (Formatter::info(file, line));
        case LogLevel::Warning: return (Formatter::warning(file, line));
        case LogLevel::Error: return (Formatter::error(file, line));
        case LogLevel::Fatal: return (Formatter::fatal(file, line));
        default: break;
    }
    return ("");
}

/* Colors */
namespace Color {

const std::string Reset = "\033[0m";
const std::string Red = "\033[31m";
const std::string Green = "\033[32m";
const std::string Yellow = "\033[33m";
const std::string Blue = "\033[34m";
const std::string Magenta = "\033[35m";

} // namespace Color

/* Formatter */
std::string const Logger::Formatter::debug(std::string file, int line) {
    std::ostringstream oss;
    oss << "[" << Color::Blue << "DEBUG" << Color::Reset << "]: (" << file
        << ":" << line << "): ";
    return oss.str();
}

std::string const Logger::Formatter::info(std::string file, int line) {
    std::ostringstream oss;
    oss << "[" << Color::Green << "INFO" << Color::Reset << "]: (" << file
        << ":" << line << "): ";
    return oss.str();
}

std::string const Logger::Formatter::warning(std::string file, int line) {
    std::ostringstream oss;
    oss << "[" << Color::Yellow << "WARNING" << Color::Reset << "]: (" << file
        << ":" << line << "): ";
    return oss.str();
}

std::string const Logger::Formatter::error(std::string file, int line) {
    std::ostringstream oss;
    oss << "[" << Color::Red << "ERROR" << Color::Reset << "]: (" << file << ":"
        << line << "): ";
    return oss.str();
}

std::string const Logger::Formatter::fatal(std::string file, int line) {
    std::ostringstream oss;
    oss << "[" << Color::Magenta << "FATAL" << Color::Reset << "]: (" << file
        << ":" << line << "): ";
    return oss.str();
}
