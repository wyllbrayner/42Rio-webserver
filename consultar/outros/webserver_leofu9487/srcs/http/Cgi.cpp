#include "webserv.hpp"

bool Cgi::is_cgi(std::map<std::string, std::string> const & cgi_map, std::string const &file_name)
{
	size_t dot_idx = file_name.find_last_of(".");

	if (dot_idx == std::string::npos)
		return false;

	std::string file_type = file_name.substr(dot_idx);
	return cgi_map.find(file_type) != cgi_map.end();
}

static void	delete_args(char **args)
{
	if (args == NULL)
		return ;

	int i = -1;
	while (args[++i] != NULL)
		delete [] args[i];

	delete [] args;
}

static char *allocate_string(std::string const &str)
{
	char *arg;
	arg = new char[str.size() + 1]();
	if (arg == NULL)
		return NULL;
	for (size_t i = 0 ; i < str.size() ; ++i)
		arg[i] = str[i];

	return arg;
}

static char **build_args(std::string const &cgi_executable, std::string const &file_name)
{
	char **args = new char*[3]();

	if (args == NULL)
		throw(InternalServerError);

	if ((args[0] = allocate_string(cgi_executable)) == NULL || (args[1] = allocate_string(file_name)) == NULL)
	{
		delete_args(args);
		throw(InternalServerError);
	}

	return args;	
}

void	add_env(char ** &env_var, size_t &idx, std::string const & var_type, std::string const &value, char ** & args)
{
	char * new_env = allocate_string(var_type + "=" + value);
	
	if (new_env == NULL)
	{
		delete_args(env_var);
		delete_args(args);
		throw(InternalServerError);
	}

	env_var[idx++] = new_env;
}

static char **build_env_var(Location const &location, HTTPRequest const &request, char **args)
{
	// source : https://www.cs.ait.ac.th/~on/O/oreilly/perl/perlnut/ch09_04.htm
	char **env_var = new char*[100]();

	if (env_var == NULL)
	{
		delete_args(args);
		throw(InternalServerError);
	}

	size_t idx = 0;
	if (request.get_method() == "POST")
	{
		add_env(env_var, idx, "CONTENT_LENGTH", ft::to_string( request.get_query_string().size() ) , args);
		add_env(env_var, idx, "CONTENT_TYPE", request.get_content_type() , args);
	}
	else if (request.get_method() == "GET")
	{
		add_env(env_var, idx, "QUERY_STRING", request.get_query_string(), args);
	}
	add_env(env_var, idx, "DOCUMENT_ROOT", location.get_root(), args);
	add_env(env_var, idx, "PATH_INFO", request.get_path(), args);
	add_env(env_var, idx, "REQUEST_METHOD", request.get_method(), args);
	add_env(env_var, idx, "SCRIPT_NAME", request.get_file_uri(), args);
	add_env(env_var, idx, "SERVER_PORT", ft::to_string(location.get_port()), args);

	// todo: if error, delete_args(args) and env_var then throw

	return env_var;
}

Cgi::Cgi(Location const &location, HTTPRequest const &request):_msg(""),_status_code(OK)
{
	std::string file_name = request.get_file_uri();

	if (!is_readable(file_name))
		throw(Forbidden);

	size_t dot_idx = file_name.find_last_of(".");

	if (dot_idx == std::string::npos)
		throw(InternalServerError);

	std::string file_type = file_name.substr(dot_idx);

	std::map<std::string, std::string>::const_iterator it = location.get_cgi().find(file_type);
	if (it == location.get_cgi().end())
		throw(InternalServerError);
	
	std::string cgi_executable = it->second;
	
	if (!is_executable(cgi_executable))
		throw(Forbidden);

	int cgi_in_fd[2], cgi_out_fd[2];

	if (pipe(cgi_in_fd) < 0 || pipe(cgi_out_fd) < 0)
		throw(InternalServerError);

	pid_t pid = fork();

	if (pid < 0)
	{
		close(cgi_in_fd[0]);
		close(cgi_in_fd[1]);
		close(cgi_out_fd[0]);
		close(cgi_out_fd[1]);
		throw(InternalServerError);
	}
	else if (pid == 0) // child process : execute cgi
	{
		dup2(cgi_out_fd[1], STDOUT_FILENO);
		dup2(cgi_in_fd[0], STDIN_FILENO);
		close(cgi_in_fd[0]);
		close(cgi_in_fd[1]);
		close(cgi_out_fd[0]);
		close(cgi_out_fd[1]);

		char **args, **env_var;

		try
		{
			args = build_args(cgi_executable, file_name);
			env_var = build_env_var(location, request, args);
		}
		catch(status_code status)
		{
			exit(MALLOC_FAIL);
		}
		
		if (execve(args[0], args, env_var) < 0)
		{
			delete_args(env_var);
			delete_args(args);
			exit(EXECVE_FAIL);
		}
	}
	else // parent process : wait for cgi to finish
	{
		close(cgi_out_fd[1]);
		close(cgi_in_fd[0]);
		if (request.get_method() == "POST")
		{
			if (write(cgi_in_fd[1], request.get_query_string().c_str(), request.get_query_string().size()) < 0)
				throw(InternalServerError);
		}
		close(cgi_in_fd[1]);

		int status = 0;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && (WEXITSTATUS(status) == EXECVE_FAIL || WEXITSTATUS(status) == MALLOC_FAIL))
			throw(InternalServerError);

		char buff[BUFFER_SIZE] = {0};
		std::string body;
		ssize_t res = 0;
		while ((res = read(cgi_out_fd[0], buff, BUFFER_SIZE - 1)) > 0)
		{
			buff[res] = '\0';
			body += buff;
		}
		close(cgi_out_fd[0]);

		std::stringstream ss(body);
		std::string tmp;
		ss >> tmp;
		if (tmp == "Status:")
		{
			ss >> tmp;
			if (is_number(tmp) && !bigger_than(tmp, "600"))
			{
				int num = string_to_int(tmp);
				if (HTTPResponse::_status_line.find(num) != HTTPResponse::_status_line.end())
				{
					_status_code = static_cast<status_code>(num);
					size_t new_line_idx = body.find_first_of("\n");
					if (new_line_idx != std::string::npos && new_line_idx + 1 < body.size())
						body = body.substr(new_line_idx + 1);
					else if (new_line_idx == body.size() - 1)
						body = "";
				}
			}
		}

		_msg = "HTTP/1.1 " + ft::to_string(_status_code) + " " + HTTPResponse::_status_line[_status_code] + "\r\n" + body;
	}
}

std::string const &Cgi::get_msg() const
{
	return _msg;
}

status_code Cgi::get_status_code() const
{
	return _status_code;
}
