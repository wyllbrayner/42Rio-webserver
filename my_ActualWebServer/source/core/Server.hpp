#pragma once
# include <iostream>
# include <cstring>
# include <unistd.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <cstdio> //perror
# include <stdlib.h> //exit()
//# include <fcntl.h> //fcntl()
# include "../config/ConfigFile.hpp"

class   Server
{
    private:
        int                     _fd_socket;
        int                     _port;
        ConfigFile              _serverConf;

    public:
        Server(void);
        Server(int  port, ConfigFile server);
        Server(const Server & copy);
        Server &operator=(const Server & src);
        ~Server(void);
        
        void                    initialize(void);
        int                     acceptCon(void) const;
        void                    closeCon(void);
        const int           &   getSocket(void) const;
        const int           &   getPort(void) const;
        const ConfigFile    &   getServerConf(void) const;
};
