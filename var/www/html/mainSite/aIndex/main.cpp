# include <signal.h>

# include "./config/ParserServer.hpp"
# include "./core/Server.hpp"
# include "./core/Webserv.hpp"
# include "Utils.hpp"

static void signalHandlerSigint(int signum);
static void closeServers(std::vector<Server> & servers);

int main(int argc, char **argv)
{
    std::vector<Server> servers;
    ParserServer        configServer;
    size_t              i;
    int                 port;

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
//        exit(1);
        i = 0;
        while (i < configServer.getNbrServers())
        {
            port = configServer.getServers()[i].getPort()[0];
            std::cout << "Initializing server number " << (i + 1) << \
            " on port " << port << std::endl;
            servers.push_back(Server(port, configServer.getServers()[i]));
//            servers[i].initialize();
            if (!servers[i].initialize())
            {
                std::cout << "Failed to initialize server " << (i + 1) \
                << " on port " << port << std::endl;
                closeServers(servers);
                break ;
            }
/*
            else
            {
                std::cout << "Server number " << (i + 1) << \
            " on port " << port << " inicializado com sucesso!" << std::endl;
            }
            servers[i].getServerConf().printConfigFile();
*/
            i++;
        }
        if (i == configServer.getNbrServers())
        {
//            exit(1);
            Webserv webserv(servers);
            closeServers(servers);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}

static void signalHandlerSigint(int signum)
{
    if (signum != SIGINT)
        return;
    Utils::_serverRunning = false;
}

static void closeServers(std::vector<Server> & servers)
{
//    std::cout << "Início | closeServers" << std::endl;
    for (size_t i = 0; i < servers.size(); i++)
    {
        std::cout << "Terminating server number " << (i + 1) << \
                " on port " << servers[i].getPort() << std::endl;
        servers[i].closeCon();
    }
//    std::cout << "Início | closeServers" << std::endl;
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
