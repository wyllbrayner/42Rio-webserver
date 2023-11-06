#include "webserv.hpp"

namespace webserv {

static void stop(int);

void webserv(std::string const& filename) {
    signal(SIGINT, stop);

    // Setting log level to info
    // LOG_LVL(LogLevel::Info);

    // reading config file an handling errors
    LOG_I("Reading config file: " << filename << "...");
    try {
        config::Parser::parse(filename);
    }
    catch (ft::InvalidFileException const& e) {
        LOG_F(filename << ": " << e.what());
        exit(EXIT_FAILURE);
    }
    catch (ft::FailedToOpenFileException const& e) {
        LOG_F(filename << ": " << e.what());
        exit(EXIT_FAILURE);
    }
    catch (config::Parser::InvalidSyntaxException const& e) {
        LOG_F("Failed parsing file " << filename << ": "
                                     << std::string(e.what()));
        exit(EXIT_FAILURE);
    }
    catch (std::exception const& e) {
        LOG_F("Found unknow exception: " << std::string(e.what()));
        exit(EXIT_FAILURE);
    }

    // starting server and handling errors
    LOG_I("Starting server...");
    try {
        Server::start();
    }
    catch (std::exception& e) {
        LOG_F("Failure while server was starting: " << std::string(e.what()));
        exit(EXIT_FAILURE);
    }

    LOG_I("Server is running. Press Ctrl+C to stop.");
    try {
        Server::run();
    }
    catch (net::ServerSocket::NoSuchConnectionException const& e) {
        LOG_F("Failed to find socket connection: " << e.what());
        exit(EXIT_FAILURE);
    }
    catch (std::exception& e) {
        LOG_F("System failure while server was running: "
              << std::string(e.what()));
        exit(EXIT_FAILURE);
    }
}

static void stop(int) {
    LOG_I("Stopping server...");
    try {
        Server::stop();
    }
    catch (std::exception& e) {
        LOG_F("Failure while server was stopping: " << std::string(e.what()));
        return;
    }
    LOG_I("Server stopped.");
}

} // namespace webserv
