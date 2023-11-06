#include "webserv.hpp"

int main(int argc, char const* argv[]) {
    (void)argc;
    (void)argv;

    std::string filename("config/webserv.conf");
    if (argc > 1) { filename = argv[1]; }
    webserv::webserv(filename);
    return (0);
}