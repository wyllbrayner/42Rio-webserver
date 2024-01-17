#include "CGI.hpp"

CGI::CGI(std::string path, const Request & request): \
    _path(path), _request(request)
{
    if (_request.getMethod() == "GET")
    {
        initEnvGET(_request.getQueryStringS());
        executeGET();
        //start timer
    }
    else if (_request.getMethod() == "POST")
    {
        initEnvPOST(_request.getQueryStringS());
        executePOST();
        //start timer
    }
}

/*
CGI     &	CGI::operator=(const CGI & src)
{
	if (this != &src)
	{
		this->_pid = src._pid;
		this->_requestFD[0] = src._requestFD[0];
		this->_requestFD[1] = src._requestFD[1];
		this->_isActive = src._isActive;
		this->_cgi_pid = src._cgi_pid;
		this->_start_time = src._start_time;
		this->_path = src._path;
		this->_response = src._response;
//		this->_request = src._request; //O request pode não ser const?
		this->_env = src._env;
	}
	return (*this);
}

CGI::CGI(const CGI & copy)
{
	*this = copy;
	return ;
}
*/

CGI::~CGI(void){}

void        CGI::initEnvGET(std::string queryString)
{
    _env.push_back(strdup(("QUERY_STRING=" + queryString).c_str()));
    _env.push_back(NULL);
}

void        CGI::initEnvPOST(std::string queryString)
{
    _env.push_back(strdup(("QUERY_STRING=" + queryString).c_str()));
    _env.push_back(strdup(("CONTENT_TYPE=" + _request.getContentType()).c_str()));
    _env.push_back(strdup(("CONTENT_LENGTH=" + _request.totalLengthS()).c_str()));
    _env.push_back(strdup(("PATH_INFO=" + this->_path).c_str()));
    _env.push_back(strdup("AUTH_TYPE=Basic"));
    _env.push_back(strdup("REQUEST_METHOD=POST"));
    _env.push_back(strdup("SERVER_PROTOCOL=HTTP/1.1"));
    _env.push_back(strdup("SERVER_SOFTWARE=Webserv/1.0"));
    _env.push_back(strdup("GATEWAY_INTERFACE=CGI/1.1"));
    _env.push_back(strdup("REDIRECT_STATUS=200"));
    _env.push_back(strdup("DOCUMENT_ROOT=./"));
    _env.push_back(strdup("TRANSLATED_PATH_INFO=.//"));
    _env.push_back(strdup(("SERVER_PORT=" + _request.returnPort()).c_str()));
    _env.push_back(strdup(("PATH_TRANSLATED=" + _path).c_str()));
    _env.push_back(strdup(("SCRIPT_NAME=" + _path).c_str()));
    _env.push_back(strdup(("REQUEST_URI=" + _request.getHost()).c_str()));
    _env.push_back(NULL);
}

void        CGI::executeGET(void)
{
    int pipefd[2];

    if(pipe(pipefd) == -1){
        std::cerr << "Erro ao criar o pipe" << std::endl;
        return ;
    }
    pid_t pid = fork();
    this->_isActive = true;
    if (pid == -1){
        std::cerr << "Error no fork" << std::endl;
        return ;
    }
    else if (pid == 0){
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        char* args[2];
        args[0] = strdup(_path.c_str());
        args[1] = NULL;
        execve(_path.c_str(), args, _env.data());
        free(args[0]);
        std::cerr << "Error ao executar execve" << std::endl;
        return ;
    } else {
        close(pipefd[1]);
        readFD(pipefd[0]);
        wait(NULL);
    }
    return ;
}

void        CGI::executePOST(void)
{
    int responseFD[2];

    if(pipe(_requestFD) == -1){
        std::cerr << "Erro ao criar o pipe" << std::endl;
        return ;
    }
    if(pipe(responseFD) == -1){
        std::cerr << "Erro ao criar o pipe" << std::endl;
        return ;
    }
    if(!writeFD(_request.returnBody()))
        return;
    pid_t pid = fork();
    
    this->_isActive = true;
    if (pid == -1){
        std::cerr << "Error no fork" << std::endl;
        return ;
    }
    else if (pid == 0){
        close(_requestFD[1]);
        close(responseFD[0]);
        dup2(_requestFD[0], STDIN_FILENO);
        close(_requestFD[0]);
        dup2(responseFD[1], STDOUT_FILENO);
        close(responseFD[1]);
        char* args[2];
        args[0] = strdup(_path.c_str());
        args[1] = NULL;
        execve(_path.c_str(), args, _env.data());
        free(args[0]);
        std::cerr << "Error ao executar execve" << std::endl;
        return;
    } else {
        close(_requestFD[0]);
        close(responseFD[1]);
        readFD(responseFD[0]);
        waitpid(pid, NULL, 0);
    }
}

void        CGI::readFD(int fd)
{
    char    buffer[BUFFER_SIZE_CGI];
    int     bytesRead;

    bytesRead = read(fd, buffer, sizeof(buffer));
    if (bytesRead > 0)
    {
        this->_response.append(buffer, bytesRead);
/*
        std::cout << "Read: ";
        std::cout << this->_response << std::endl;
        std::cout << std::endl;
*/
    }
    else
    {
        std::cerr << "Erro na leitura da resposta do filho" << std::endl;
        //fazer uma classe de log
    }
}

std::string CGI::getBody(void) const
{
    return this->_response;
}

void        CGI::routineCheck(void)
{
    time_t  current_time = time(NULL);

    while(_isActive)
    {
        if(current_time - this->_start_time >= TIME_LIMIT)
        {
            kill(this->_cgi_pid, SIGKILL);
            this->_isActive = false;
        }
    }
}

bool        CGI::writeFD(std::string body)
{
    size_t  bytesWritten;
    int bytes;

    bytesWritten = 0;
    while (bytesWritten < body.length())
    {
        bytes = write(this->_requestFD[1], body.c_str() + bytesWritten, \
                    body.length() - bytesWritten);
        if (bytes == -1)
        {
            std::cerr << "error";
            return (false);
        }
        bytesWritten += bytes;
    }
    return (true);
}