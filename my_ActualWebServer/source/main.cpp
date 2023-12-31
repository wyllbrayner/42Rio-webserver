#include <signal.h>

#include "./core/Webserv.hpp"
#include "Utils.hpp"

static void signalHandlerSigint(int signum)
{
    if (signum != SIGINT) return;
    Utils::_serverRunning = false;
}

int main(int argc, char **argv) {
    ParserServer	configServer;

    signal(SIGINT, signalHandlerSigint);
    try
    {
        if (argc > 2)
            throw Error::InvalidArg();
        if (argc == 1)
            configServer.createServer();
        else
            configServer.createServer(argv[1]);
//        configServer.print();
        std::vector<Server> servers;
        for (size_t i = 0; i < configServer.getNbrServers(); i++) {
            int port = configServer.getServers()[i].getPort()[0];
            std::cout << "Initializing server number " << (i + 1) << \
            " on port " << port << std::endl;
            servers.push_back(Server(port, configServer.getServers()[i]));
            servers[i].initialize();
//            servers[i].getServerConf().printConfigFile();
        }
//        exit(1);
        Webserv webserv(servers);
        for (size_t i = 0; i < servers.size(); i++)
        {
            std::cout << "Terminating server number " << (i + 1) << " on port " << servers[i].getPort() << std::endl;
            servers[i].closeCon();
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}

/*
        size_t i = 0;
        size_t j;
        size_t k;
        while (i < configServer.getNbrServers()) {
            j = 0;
            while (j < servers[i].getServerConf().getLocation().size()) {
                k = 0;
                while ( k < servers[i].getServerConf().getLocation()[j].getIndex().size()) {
                    std::cout << "in server " << i << " Location: " << j << " index para k: " << k << " é: " << servers[i].getServerConf().getLocation()[j].getIndex()[k] << std::endl;
                    k++;
                }
                j++;
            }
//            std::cout << "in server " << (i + 1) << " Port: " << servers[i].getServerConf().getPort() << std::endl;
            i++;
        }
        return (0);
*/
