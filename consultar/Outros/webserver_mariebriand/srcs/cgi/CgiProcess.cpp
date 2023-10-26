/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiProcess.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 16:40:07 by mabriand          #+#    #+#             */
/*   Updated: 2022/02/21 10:19:53 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiProcess.hpp"

CgiProcess::CgiProcess(Request *current_request, Server *current_server) : _myEnv(NULL)
{
	this->_request = current_request;
	this->_server = current_server;
}
CgiProcess::~CgiProcess()
{
	this->clearEnv();
}

void				CgiProcess::init()
{
	this->set_envVars();
	this->set_myEnv();

	int i = 0;

	while (this->_myEnv[i] != NULL)
		++i;
}

bool				CgiProcess::isCgiNeeded()
{
	std::string	temp = this->_request->getUrl();
	size_t		size = temp.size();
	size_t		idx = temp.find_last_of(".", size);
	std::string ext(temp.substr(idx, size - idx));

	if (idx == std::string::npos)
		return (false);
	else if (ext.compare(".php") != 0)
		return (false);
	return (true);
}

void				CgiProcess::set_envVars()
{
	this->_env_map["REDIRECT_STATUS"]	=	"200"; //for php-cgi
	if (this->_request->getMethod() == "GET")
		this->_env_map["CONTENT_LENGTH"]=	std::to_string(this->_request->get_queryString().size()); //query string pareil ?
	else if (this->_request->get_contentLength() != "")
		this->_env_map["CONTENT_LENGTH"]=	this->_request->get_contentLength(); //query string pareil ?
	else
		this->_env_map["CONTENT_LENGTH"]=	std::to_string(this->_request->getBody().size()); //query string pareil ?	=	iToString(this->_req.body_size);	// content-length de la requete
	this->_env_map["CONTENT_TYPE"]		=	this->_request->get_contentType();	// content-type de la requete (POST)
	this->_env_map["GATEWAY_INTERFACE"]	=	"CGI/1.1";	// version du CGI qu'utilise le server
	this->_env_map["PATH_INFO"]			=	this->_request->getBaseUrl();	// derniere partie de l'URI apres le host
	this->_env_map["PATH_TRANSLATED"]	=	this->_request->getUrl();	// adresse reelle du script (idem PATH_INFO pour nous)
	this->_env_map["QUERY_STRING"]		=	this->_request->get_queryString();	// Contient tout ce qui suit le « ? » dans l'URL envoyée par le client.
	this->_env_map["REMOTE_ADDR"]		=	this->_request->getBlock()->get_host();	// adress ip du client
	this->_env_map["REQUEST_METHOD"]	=	this->_request->getMethod();	// GET ou POST ou ...
	this->_env_map["REQUEST_URI"]		=	this->_request->getBaseUrl(); // --> For the 42 tester
	this->_env_map["SCRIPT_NAME"]		=	this->_request->getUrl();	// full path du fichier de script
	this->_env_map["SCRIPT_FILENAME"]	=	this->_request->getUrl();	// full path du fichier de script
	this->_env_map["SERVER_NAME"]		=	this->_request->getBlock()->get_host();	// DNS ou IP du server (hostname)
	this->_env_map["SERVER_PORT"]		=	std::to_string(this->_request->getBlock()->get_port());	// port ayant reçu la requête
	this->_env_map["SERVER_PROTOCOL"]	=	this->_request->getProtocolVersion();	// protocol HTTP (toujours HTTP/1.1 ?)
	this->_env_map["SERVER_SOFTWARE"]	=	"Serv";
	this->_env_map["UPLOAD_DIR"]		=	"uploads/";
	this->_env_map["DOCUMENT_ROOT"]		=	this->get_cwd();  //for php-cgi
	this->_env_map["SERV_NAME"]			=	this->_request->getBlock()->get_server_name();
}

void				CgiProcess::set_cgiOutput(std::vector<unsigned char> body)
{
	size_t i = 0;
	while (i < body.size())
	{
		this->_cgi_output.push_back(body[i]);
		++i;
	}
	// std::vector<unsigned char> toto = this->_cgi_output;
	// for (std::vector<unsigned char>::iterator it = toto.begin(); it != toto.end(); it++)
	// 	std::cout << YELLOW << *it;
	// std::cout << '\n' << NC;

	// std::cout << this->_cgi_output.size() << '\n';
}

std::string			CgiProcess::get_cwd(void)
{
	char cwd[256];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (std::string("./"));
	else
		return (std::string(cwd));
}

void				CgiProcess::set_myEnv()
{
	size_t		size = this->_env_map.size();
	int			i = 0;
	this->_myEnv = (char **)malloc(sizeof(char *) * (size + 1));
	if (!this->_myEnv)
		throw (CGI_ERR_PROCESS("Mallo failed"));

	for (std::map<std::string, std::string>::iterator it = this->_env_map.begin(); it != this->_env_map.end(); it++)
	{
		std::string tmp = it->first + '=' + it->second;
		this->_myEnv[i] = strdup(tmp.c_str());
		// std::cout << GREEN << this->_myEnv[i] << NC << std::endl;
		i++;
	}
	this->_myEnv[size] = NULL;
}

std::vector<unsigned char>	CgiProcess::get_cgiOutput(){ return (this->_cgi_output); }

void				CgiProcess::clearEnv()
{
	if (this->_myEnv != NULL)
	{
		for (int i = 0; this->_myEnv[i]; ++i)
			free(this->_myEnv[i]);
		free(this->_myEnv);
		this->_myEnv = NULL;
	}
}

void				storeBuffer(std::vector<unsigned char> &body, const char *buf, int len)
{
	int i = 0;

	if (len < 65536 && len != 0)
		len++;

	while(i < len)
		body.push_back(buf[i++]);
}

void				treat_body(std::vector<unsigned char> *body)
{
	size_t i = 0;

	while (i < body->size())
	{
		size_t j = i;
		while (j < body->size() && (*body)[j] != '\n' && (*body)[j] != '\r')
			j++;
		std::string line(body->begin() + i, body->begin() + j);
		if (line.compare("X-Powered-By: PHP/8.1.2") == 0)
			break;
		i = j + 1;
	}
	body->erase(body->begin(), body->begin() + i);
}

int					CgiProcess::exeCgiProgram()
{
	/* Le script prend des données en entrée et écrit son resultat dans STDOUT.
	Dans le cas de GET, les données d'entrées sont dans la var d'env QUERY_STRING,
	Dans le cas de POST, les données sont lues depuis STDIN (depuis le body de la requete).
	Comme le scrit écrit dans stdout, il faut lire stdout et l'enregistrer dans une variable,
	variable qui sera retournée par la fonction execScript() et utilsée pour contruire le bdy de la réponse.
	*/

	int srvToCgi_fd[2]; // Pipe Server --> CGI (for sending body to cgi)
	int cgi_fd; // share file btw server and CGI (for receiving cgi output)

	cgi_fd = open("/tmp/cgi_file", O_RDWR | O_CREAT | O_APPEND | O_TRUNC, 0666); // if file with that name already exists, O_TRUNC will "erase" it
	if (cgi_fd == -1)
	{
		std::cout << "open error" << std::endl;
		return -1;
	}
	if (pipe(srvToCgi_fd) == -1)
	{
		std::cerr << "pipe() srvToCgi failed, errno: " << errno << std::endl;
		return -1;
	}

	int pid = fork();
	if (pid == -1)
	{
		std::cerr << "fork process failed" << std::endl;
		return -1;
	}
	else if (pid == 0)
	{
		close(srvToCgi_fd[1]);  /* Ferme l'extrémité d'ecriture inutilisée */
		if (dup2(cgi_fd, STDOUT_FILENO) == -1)
		{
			std::cout << "problem with dup: " << errno << std::endl;
			return -1;
		}
		if (dup2(srvToCgi_fd[0], STDIN_FILENO) == -1)
		{
			std::cout << "problem with dup: " << errno << std::endl;
			return -1;
		}
		if (dup2(cgi_fd, STDERR_FILENO) == -1)
		{
			std::cout << "problem with dup: " << errno << std::endl;
			return -1;
		}
		close(cgi_fd);
		close(srvToCgi_fd[0]);

		std::string	cgi_path = this->_server->get_conf()->get_cgi_path();

		char * argv[3] = {
			const_cast<char*>(cgi_path.c_str()),
			const_cast<char*>(this->_request->getBaseUrl().c_str()),
			(char *)0
		};

		if (execve(argv[0], &argv[0], this->_myEnv) < 0) /* Le script écrit dans STDOUT */
		{
			close(srvToCgi_fd[0]);  /* Ferme l'extrémité de lecture après utilisation par le fils */
			_exit(1);
		}
	}
	else
	{
		close(srvToCgi_fd[0]);  /* Ferme l'extrémité de lecture inutilisée */

		// 1. write to cgi
		// std::vector<unsigned char> toto = this->_request->getBody();
		// for (std::vector<unsigned char>::iterator it = toto.begin(); it != toto.end(); it++)
		// 	std::cout << YELLOW << *it;
		// std::cout << '\n' << NC;

		if (!this->_request->getVecBody().empty())
		{
			size_t ret_write =  write(srvToCgi_fd[1], &this->_request->getVecBody()[0], this->_request->getVecBody().size());

			if (ret_write < 0)
				return (-1);
			else if (ret_write != this->_request->getVecBody().size())
				return (-1);
		}
		close(srvToCgi_fd[1]);

		// 2. wait for cgi to finish
		// do waitpid before read, so parent process will access shared file only when child is done with it
		int status;
		if (waitpid(pid, &status, 0) == -1)
			return -1;
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) == 1)
				return -1;
		}

		// 3. Read from cgi and fill outputs
		lseek(cgi_fd, 0, SEEK_SET); // reposition file offset at begining
		int ret = 65536;
		char	buf[65536];
		std::vector<unsigned char> body;
		while (ret == 65536)
		{
			memset(buf, 0, 65536);
			if ((ret = read(cgi_fd, buf, 65536)) < 0)
				return -1;
			storeBuffer(body, buf, ret);
		}

		close(cgi_fd);

		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) == 1)
				return -1;
		}

		this->set_cgiOutput(body);
		// treat_body(&this->_cgi_output);
	}
	return 0;
}
