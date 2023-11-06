#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "utils/ft_exceptions.hpp"
#include "utils/smt.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace LogLevel {

enum level_t { Debug, Info, Warning, Error, Fatal, None };

}

class Logger {
    public:

        static Logger& getInstance(void);

        int            getLevel(void) const;
        std::ofstream& getFile(void);

        void setLevel(LogLevel::level_t level);
        void setFile(std::string filename);

        std::string log(LogLevel::level_t lvl, std::string filename, int line);

        class Formatter {
            public:

                static std::string const debug(std::string file, int line);
                static std::string const info(std::string file, int line);
                static std::string const warning(std::string file, int line);
                static std::string const error(std::string file, int line);
                static std::string const fatal(std::string file, int line);
        };

    private:

        Logger(LogLevel::level_t lvl = LogLevel::Debug);
        ~Logger(void);

        LogLevel::level_t m_lvl;
        bool              m_ffile;
        std::ofstream     m_file;
};

#define LOG_LVL(L)  Logger::getInstance().setLevel(L)
#define LOG_FILE(F) Logger::getInstance().setFile(F)

#define LOG(M, L)                                                              \
    if (Logger::getInstance().getLevel() <= L) {                               \
        std::cout << Logger::getInstance().log(L, __FILE__, __LINE__) << M     \
                  << std::endl;                                                \
    }

#define FLOG(M, L)                                                             \
    if (Logger::getInstance().getLevel() <= L) {                               \
        Logger::getInstance().getFile()                                        \
            << Logger::getInstance().log(L, __FILE__, __LINE__) << M           \
            << std::endl;                                                      \
    }

#define LOG_D(M) LOG(M, LogLevel::Debug)
#define LOG_I(M) LOG(M, LogLevel::Info)
#define LOG_W(M) LOG(M, LogLevel::Warning)
#define LOG_E(M) LOG(M, LogLevel::Error)
#define LOG_F(M) LOG(M, LogLevel::Fatal)

#define FLOG_D(M) FLOG(M, LogLevel::Debug)
#define FLOG_I(M) FLOG(M, LogLevel::Info)
#define FLOG_W(M) FLOG(M, LogLevel::Warning)
#define FLOG_E(M) FLOG(M, LogLevel::Error)
#define FLOG_F(M) FLOG(M, LogLevel::Fatal)

#endif /* LOGGER_HPP */
