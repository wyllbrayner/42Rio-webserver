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
       // configServer.print();
        i = 0;
        while (i < configServer.getNbrServers())
        {
            port = configServer.getServers()[i].getPort()[0];
            std::cout << "Initializing server number " << (i + 1) << \
            " on port " << port << std::endl;
            servers.push_back(Server(port, configServer.getServers()[i]));
            if (!servers[i].initialize())
            {
                std::cout << "Failed to initialize server " << (i + 1) \
                << " on port " << port << std::endl;
                break ;
            }
            i++;
        }
        if (i == configServer.getNbrServers())
            Webserv webserv(servers);
        closeServers(servers);
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
    for (size_t i = 0; i < servers.size(); i++)
    {
        std::cout << "Terminating server number " << (i + 1) << \
                " on port " << servers[i].getPort() << std::endl;
        servers[i].closeCon();
    }
}