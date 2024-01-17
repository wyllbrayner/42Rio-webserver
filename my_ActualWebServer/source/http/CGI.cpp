#include "CGI.hpp"

CGI::CGI(std::string path, Request const & request): 
    _path(path),
    _request(request)
{
//    std::cout << "Header: " << _request.returnHeader() << std::endl;
    if (_request.getMethod() == "GET")
    {
/*
        std::cout << "É UM GET COM CGI " << _path << std::endl;
        std::cout << "Path: " << _path << std::endl;
        std::cout << "Request: " << _request.getQueryStringS() << std::endl;
        std::cout << "Location:" << _request.getRequestedInf() << std::endl;
*/

        initEnvGET(_request.getQueryStringS());
        //read fd
        executeGET();
        //start timer
    } else if (_request.getMethod() == "POST"){
        //creat fd body;
        //creat variavel para fd de response
/*
*/
        std::cout << "É UM POST COM CGI " << _path << std::endl;
        std::cout << "Path: " << _path << std::endl;
        std::cout << "Request: " << _request.getQueryStringS() << std::endl;
        std::cout << "Location:" << _request.getRequestedInf() << std::endl;

        initEnvPOST(_request.getQueryStringS());
        executePOST();
        //start timer
        std::cout << _request.returnBody() << std::endl;

    }
}

CGI::~CGI(){}

void CGI::initEnvGET(std::string queryString){
//    std::cout << "início | initEnvGET queryString: " << queryString << std::endl;
    _env.push_back(strdup(("QUERY_STRING=" + queryString).c_str()));
    _env.push_back(NULL);

/*
    size_t i = 0;
    std::cout << "_env size: " << this->_env.size() << std::endl;
    while (i < this->_env.size())
    {
        std::cout << "_env[" << i << "]: " << this->_env[i] << std::endl;
        i++;
    }
    std::cout << "fím    | initEnvGET queryString: " << queryString << std::endl;
    exit(1);
*/
}

void CGI::initEnvPOST(std::string queryString){
    std::cout << "início | initEnvPOS  queryString: " << queryString << std::endl;    
   
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
    std::cout << "_env.size: " << this->_env.size() << std::endl;
/*
    for (size_t i = 0; i < this->_env.size(); i++)
    {
        std::cout << "_env[" << i << "]: " << this->_env[i] << std::endl;
    }
*/
    std::cout << "Fim    | initEnvPOS" << std::endl;
}

void CGI::executeGET(){
    
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
        return;
    } else {
        close(pipefd[1]);
        readFD(pipefd[0]);
        wait(NULL);
    }
    return;
}

//creat execute execve for python file
void CGI::executePOST(void){
    int responseFD[2];

    std::cout << "início | executePOST" << std::endl;    

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

        //writeFD(_requestFD[1]);
        readFD(responseFD[0]);
        waitpid(pid, NULL, 0);
    }
    std::cout << "Fim | executePOST" << std::endl;
}

void CGI::readFD(int fd){
    char buffer[BUFFER_SIZE_CGI];
    int bytesRead = read(fd, buffer, sizeof(buffer));
    if (bytesRead > 0) {
        std::cout << "Read: ";
        this->_response.append(buffer, bytesRead);
        std::cout << this->_response << std::endl;
        std::cout << std::endl;
    } else {
        std::cerr << "Erro na leitura da resposta do filho" << std::endl;
        //fazer uma classe de log
    }
}

std::string CGI::getBody(void) const{
    return this->_response;
}

void CGI::routineCheck(void){
    time_t current_time = time(NULL);

    while(_isActive){
        if(current_time - this->_start_time >= TIME_LIMIT){
            kill(this->_cgi_pid, SIGKILL);
            this->_isActive = false;
        }
    }
}

//write fd with while loop, write all body
bool CGI::writeFD(std::string body){
    size_t bytesWritten = 0;

    while (bytesWritten < body.length()){
        int bytes = write(this->_requestFD[1], body.c_str() + bytesWritten, body.length() - bytesWritten);
        if (bytes == -1){
            std::cerr << "error";
            return false;
        }
        bytesWritten += bytes;
    }
    return true;

}
