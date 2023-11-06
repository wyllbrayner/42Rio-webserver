/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 16:52:54 by vmoreau           #+#    #+#             */
/*   Updated: 2022/02/07 12:50:30 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/webserv.hpp"
#include "conf/confpars.hpp"
#include "request/Request.hpp"
#include "response/Response.hpp"
#include "server/Server.hpp"

/*
	FUNCTION SIGNAL_HANDLER:
		Catch if a CTRL-C is send in terminal and set static int server_is_alive to 0
		server_is_alive is the condition for the infinit loop which make the server run
*/
void signal_handler(int signum)
{
	if (signum == SIGINT)
		Server::server_is_alive = 0;
	if (signum == SIGPIPE)
		std::cout << "SP caught" << std::endl;
}

int main(int ac, char **av)
{
	if (ac <= 2)
	{
		confpars *conf = new confpars();

		if (signal(SIGINT, signal_handler) == SIG_ERR)
			return (1);
		if (signal(SIGPIPE, signal_handler) == SIG_ERR)
			return (1);
		try
		{
			if (ac == 1)
				conf->open_fc(CONF_DEFAULT_PATH);
			else
				conf->open_fc(av[1]);

			conf->pars_fc(conf->get_fd());					// Parsing conf file
			conf->close_fc();

			Server *serv = new Server();					// Alloc a new class server to avoid stack overflow

			serv->Server_init(conf, conf->get_server());
			serv->Server_launch();
			delete conf;
			delete serv;
		}
		catch(const std::exception &e)
		{
			std::cerr << RED << "ERROR: " << NC << e.what() << '\n';
			delete conf;
			return (EXIT_FAILURE);
		}
	}
	else
		std::cerr << RED << "ERROR: " << NC << "too many argument\n";

	std::cout << "\n\nSERVER ENDED\n\n";

	return (EXIT_SUCCESS);
}
