#pragma once
# include <iostream>
# include <unistd.h>
# include <sys/socket.h>
# include "../config/ConfigFile.hpp"

class   Server
{
    private:
        int                     _fd_socket;
        int                     _port;
        ConfigFile              _serverConf;

    public:
        Server(int port, ConfigFile server);
        Server(const Server & copy);
        Server              &   operator=(const Server & src);
        ~Server(void);

        bool                    initialize(void);
        int                     acceptCon(void) const;
        void                    closeCon(void);
        const int           &   getSocket(void) const;
        const int           &   getPort(void) const;
        const ConfigFile    &   getServerConf(void) const;
};
