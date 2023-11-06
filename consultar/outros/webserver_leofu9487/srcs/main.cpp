#include "../includes/webserv.hpp"

int main(int ac, char **av)
{
	if (ac != 2)
	{
		print_log("Fatal : Incorrect Argument Amount");
		std::exit(EXIT_FAILURE);
	}

	std::vector<ServerInfo> serverlist;
	std::map<int, ServerInfo *> fd_of_servers;

	try
	{
		parse_servers(serverlist, av[1]);
		#ifdef TEST
		serverlist.push_back(ServerInfo());
		#endif
		// std::cerr << serverlist.size() <<"\n";
		// for (std::vector<ServerInfo>::iterator it = serverlist.begin() ; it != serverlist.end() ; ++it)
		// {
		// 	it->print();
		// }
		
		create_listening_sockets(fd_of_servers, serverlist);
		start_server(fd_of_servers);
		// close_sockets(fd_of_servers); also close clients ?
	}
	catch(const std::exception& e)
	{
		print_log("Fatal : " + std::string(e.what()));
		std::exit(EXIT_FAILURE);
	}

	return (0);
}
