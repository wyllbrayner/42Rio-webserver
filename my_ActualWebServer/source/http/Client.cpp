#include "Client.hpp"

Client::Client(const Request &request, Response &response) : 
    _request(request), 
    _response(response),
    _statusCode(0)
{
//    std::cout << "inicio | Client" << std::endl;
    this->_code = "0";
//    std::cout << "inicio | Client _code: " << this->_code << std::endl;
    this->getMethod();
}

Client::~Client() {}

std::string Client::getMethod(void)
{
    std::string pagePath;
    std::ostringstream page;

    pagePath = this->fileRequested();
    if (pagePath.compare(0, 2, "./") == 0)
    {
        std::ifstream file(pagePath.c_str());
        if (!file)
        {
            this->_response.httpMessage.append("HTTP/1.1 404 Not Found\r\n\r\n");
            return "";
        }
        // Here execute	methods or CGI
        page << file.rdbuf();
        file.close();
    }
    else
        page << pagePath;

    /* std::ostringstream resp;
    resp << "Content-Length: " << "19" << "\n";
    resp << "<h1>webserver</h1>\n"; */
    page.flush();
    std::string text;
    int lenPage = page.str().size();
//    text.append("HTTP/1.1 ").append(this->getCode()).append(" OK\r\n");
    text.append("HTTP/1.1 200 OK\r\n");
    text.append("Content-Type: text/html\r\n");
    std::string content_len;
    std::stringstream sstream;
    sstream << lenPage;
    content_len.append("Content-Length: ").append(sstream.str()); //para fazer funcinar na 42!
//    std::string content_len = "Content-Length: " + lenPage; // na 42, estava dando erro!
    content_len += "\r\n\n";
    text.append(content_len);
    text.append(page.str());
    /* text.append("<html>\n");
    text.append("<body>\n");
    text.append("<h1>Hello, World!</h1>\n");
    text.append("</body>\n");
    text.append("</html>\n" );*/
    // std::cout << "Response client: \n" << text << "\n\n";
    this->_response.httpMessage = text;
    _statusCode = 200;
    return (this->_response.httpMessage);
}

const std::string & Client::getCode(void) const {
    return this->_code;
}

int Client::getStatusCode(void){
    return this->_statusCode;
}
/*
std::string Client::readFile(std::string name){
    
    std::ifstream file(name.c_str());

    if (file.is_open()){
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string contents = buffer.str();

        std::cout << contents << "\n";
        file.close();
        _statusCode = 200;
        return contents;
    } else {
        _statusCode = 404;
        return "404 Not Found";
    }
}
*/

std::string	Client::fileRequested(void)
{
//    std::cout << "inicio | fileRequested" << std::endl;
    std::vector<std::string>::iterator  it00;
    std::vector<std::string>            tmpVec00;
    std::vector<std::string>            tmpVec01;
    std::string                         fileRequested;
    size_t                              i;
    size_t                              j;
    size_t                              k;

	tmpVec00 = this->_request.getServerConf().getIndex();
	it00 = find(tmpVec00.begin(), tmpVec00.end(), this->_request.getRequestedInf());
/*
    if (this->_request.getRequestedInf().empty())
        std::cout << "\trequest Index: vazio" << std::endl;
    else
        std::cout << "\trequest Index: " << this->_request.getRequestedInf() << std::endl;
    j = 0;
    while (j < this->_request.getServerConf().getIndex().size())
    {
        std::cout << "\tconfigF Index: " << \
                this->_request.getServerConf().getIndex()[j++] << std::endl;
    }
    if ((this->_request.getLocation() == this->_request.getServerConf().getRoot()))
        std::cout << "    ACHOU\t location request: " << this->_request.getLocation() << " com location conf: " << this->_request.getServerConf().getRoot() << std::endl;
    else
    {
        std::cout << "NÃO ACHOU\t location request: " << this->_request.getLocation() << " com location conf: " << this->_request.getServerConf().getRoot() << std::endl;
    }
	if  ((it00 != tmpVec00.end()) || this->_request.getRequestedInf().empty())
        std::cout << "    ACHOU\t index request: " << this->_request.getRequestedInf() << std::endl;
    else
        std::cout << "NÃO ACHOU\t index request: " << this->_request.getRequestedInf() << std::endl;
*/

	if ((this->_request.getLocation() == this->_request.getServerConf().getRoot()) && \
		((it00 != tmpVec00.end()) || this->_request.getRequestedInf().empty()))
	{
//        std::cout << "Dentro do if fileRequested: " << fileRequested << std::endl;
		if (it00 != tmpVec00.end())
			fileRequested.append(".").append(this->_request.getLocation()).append(this->_request.getRequestedInf());
		else
			fileRequested.append(".").append(this->_request.getLocation()).append(tmpVec00[0]);
        this->_statusCode = 200;
        this->_code = "200";
	}
	else
	{
/*
        std::cout << "Dentro do else fileRequested: " << fileRequested << std::endl;
        std::cout << "RequestedInf                : " << this->_request.getRequestedInf() << std::endl;
        std::cout << "RequestedMethod             : " << this->_request.getMethod() << std::endl;
        std::cout << "RequestedLocation           : " << this->_request.getLocation() << std::endl;
*/
		i = 0;
		while (i < this->_request.getServerConf().getLocation().size())
		{
//            std::cout << "Em Location n:\t" << (i + 1) << " Path: " << this->_request.getServerConf().getLocation()[i].getPath() << std::endl;
			if (this->_request.getServerConf().getLocation()[i].getPath() == this->_request.getLocation())
            {
                tmpVec00 = this->_request.getServerConf().getLocation()[i].getMethods();
                j = 0;
                while ((j < tmpVec00.size()) && (tmpVec00[j].compare(this->_request.getMethod()) != 0))
                {
//                    std::cout << "Método[" << j << "]: |" << tmpVec00[j] << "|" << std::endl;
                    j++;
                }
                if ((j < tmpVec00.size()))
                {
//                    std::cout << "Méthodo " << tmpVec00[j] << "     autorizado!!!" << std::endl;
                    tmpVec01 = this->_request.getServerConf().getLocation()[i].getIndex();
                    k = 0;
                    while (k < tmpVec01.size() && (tmpVec01[k].compare(this->_request.getRequestedInf()) != 0))
                    {
//                        std::cout << "índice[" << k << "]: |" << tmpVec01[k] << "|" << std::endl;
                        k++;
                    }
//                    std::cout << "size de RequestedInf: " << this->_request.getRequestedInf() << " é: " << this->_request.getRequestedInf().size() << std::endl;
                    if (k < tmpVec01.size())
                    {
//                        std::cout << "Achou RequestedInf(): " << this->_request.getRequestedInf() << std::endl;
                        fileRequested.append(".").append(this->_request.getServerConf().getLocation()[i].getReturn()).append(this->_request.getRequestedInf());
                    }
                    else if (this->_request.getRequestedInf().empty())
                    {
//                        std::cout << "RequestedInf() está vazio: " << std::endl;
                        fileRequested.append(".").append(this->_request.getServerConf().getLocation()[i].getReturn()).append(this->_request.getServerConf().getLocation()[i].getIndex()[0]);
                    }
                    else
                    {
//                        std::cout << "RequestedInf: " << this->_request.getRequestedInf() << " não identificado!" << std::endl;
                        fileRequested.append("Error404");
                    }
                    if (this->_request.getServerConf().getLocation()[i].getAutoIndex())
                    {
//                        std::cout << "Autoindex" << std::endl;
                        fileRequested.append("autoindex");
                    }
                    this->_statusCode = 200;
                    this->_code = "200";
                    break ;
                }
                else
                {
                    fileRequested.append("Error405");
//                    std::cout << "Méthodo não autorizado!!!" << std::endl;
                    break ;
                }
            }
			i++;
		}
	}
//    std::cout << "fileRequested: " << fileRequested << std::endl;
	if (fileRequested.find("autoindex") != std::string::npos)
    {
//        std::cout << "fileRequested is     autoindex pré: " << fileRequested << std::endl;
        fileRequested.erase(fileRequested.find("autoindex"));
        if (fileRequested.compare(0, 5, "Error") != 0)
        {
//            std::cout << "fileRequested is not a error page: " << fileRequested << std::endl;
           	if (Utils::getTypePath(fileRequested) != 1)
            {
//                std::cout << fileRequested << " não exite no servidor | Chamar autoIndex!!!" << std::endl;
                this->buildAutoindexPage(fileRequested.erase(fileRequested.rfind("/") + 1));
            }
//            else
//                std::cout << fileRequested << "     exite no servidor" << std::endl;                
        }
//        else
//            std::cout << "fileRequested is     a error page: " << fileRequested << std::endl;
//        std::cout << "fileRequested is     autoindex pós: " << fileRequested << std::endl;
    }
    else if ((this->_request.getMethod().compare(0, 6, "DELETE") == 0) && \
        (this->_request.getQueryString().size() > 0) && !fileRequested.empty() && (fileRequested.compare(0, 5, "Error") != 0))
    {
//        std::cout << "######This is a DELETE##################" << std::endl;
        std::map<std::string, std::string> tmpMap;
        std::map<std::string, std::string>::iterator itMap;
        tmpMap = this->_request.getQueryString();
        itMap = tmpMap.begin();
//        std::cout << "o tamanho de tmpMap é: " << tmpMap.size() << " first: " << itMap->first << " second: " << itMap->second << std::endl;
        this->buildDeleteFile(fileRequested, itMap->second);
    }
//    else
//    {
//        std::cout << "a requisição é do método: " << this->_request.getMethod() << std::endl;
//        std::cout << "fileRequested is not autoindex: " << fileRequested << std::endl;
//    }
//    std::cout << "fileRequested is empty or Error: " << fileRequested << std::endl;
	if (fileRequested.empty() || fileRequested.compare(0, 5, "Error") == 0)
    {
//        std::cout << "fileRequested is empty or Error" << std::endl;
        if (fileRequested.empty())
            this->searchErrorFile(fileRequested, "404");
        else
            this->searchErrorFile(fileRequested, fileRequested.substr(5, 3));
    }
//    std::cout << "Verificar se " << fileRequested << " exite no servidor" << std::endl;
    if ((fileRequested.find("keyPage") == std::string::npos) && \
                (Utils::getTypePath(fileRequested) != 1) && \
                (!this->_request.getServerConf().getLocation()[i].getAutoIndex()))
	{
//        std::cout << fileRequested << " não exite no servidor e não é autoindex" << std::endl;
        if (fileRequested.compare(3, 7, "Default") != 0)
        {
            this->searchErrorFile(fileRequested, "500");
            if (Utils::getTypePath(fileRequested) != 1)
            {
//                std::cout << fileRequested << " também não exite no servidor" << std::endl;
                this->searchErrorFile(fileRequested, "500");
            }
        }
//        else
//            std::cout << "fileRequested is: " << fileRequested << std::endl;
	}
    if (fileRequested.find("keyPage") != std::string::npos)
        fileRequested.erase(fileRequested.find("keyPage"));
//    std::cout << "final | fileRequested: " << fileRequested << std::endl;
//    std::cout << "final  | fileRequested" << std::endl;
	return (fileRequested);
}

void    Client::searchErrorFile(std::string & fileRequested, std::string errorCode)
{
//    std::cout << "inicio | searchErrorFile " << fileRequested << " and code: " << errorCode << std::endl;
    size_t  i;
    
    i = 0;
    while (i < this->_request.getServerConf().getErrorPage().size())
    {
//        std::cout << "errorPage[" << i << "]: " << this->_request.getServerConf().getErrorPage()[i] << std::endl;
        if (this->_request.getServerConf().getErrorPage()[i].find(errorCode) != std::string::npos)
            break ;
        i++;
    }
    if (i == this->_request.getServerConf().getErrorPage().size())
    {
    	fileRequested.erase();
        this->buildDefaultErrorPage(fileRequested, errorCode);
    }
    else
        fileRequested.erase().append(".").append(\
                    this->_request.getServerConf().getErrorPage()[i]);
    this->_statusCode = Utils::atoi(errorCode);
    this->_code = errorCode;
//    std::cout << "final  | searchErrorFile " << fileRequested << " and statusCode: " << this->_statusCode << std::endl;
}

void    Client::buildDeleteFile(const std::string & path, const std::string & idValue)
{
//    std::cout << "start | buildDeleteFile path: " << path << " id Value: " << idValue << std::endl;
	std::ifstream	ifs;
    std::ofstream   ofs;
	std::string		line;
	std::string		page;

	ifs.open(path.c_str());
	if (ifs.is_open())
	{
//        std::cout << "início do 1º if | arquivo: " << path << " está aberto coomo leitura" << std::endl;
		while(std::getline(ifs, line))
		{
            if (line.find(idValue, 0) != std::string::npos)
            {
        		while(std::getline(ifs, line))
		        {
                    if (line.find("</div>", 0) != std::string::npos)
                    {
                        std::getline(ifs, line);
                        std::getline(ifs, line);
                        break ;
                    }
                }
            }
            page += line.append("\n");
		}
		ifs.close();
//        std::cout << "Final  do 1º if | arquivo: " << path << " está aberto coomo leitura" << std::endl;
//        std::cout << "page\n" << page << std::endl;
	}
//    std::cout << "Inserir o conteudo para o arquivo: " << path << std::endl;
    if (!page.empty())
    {
        ifs.open(path.c_str());
        if (ifs.is_open())
        {
            ofs.open(path.c_str(), std::ios::out | std::ios::trunc);
            if (ofs.is_open())
            {
//                std::cout << "início do 2º if | arquivo: " << path << " está aberto coomo leitura" << std::endl;
                ofs << page;
                ofs.close();
//                std::cout << "Final  do 2º if | arquivo: " << path << " está aberto coomo leitura" << std::endl;
            }
//            else
//                std::cout << "Não abriu o truncat do arquivo: " << path << std::endl;
            ifs.close();
        }
    }
//    std::cout << "end   | buildDeleteFile" << std::endl;
}

void    Client::buildAutoindexPage(std::string & path)
{
//    std::cout << "Início | buildAutoindex de: " << path << std::endl;
    std::string delimeter;
    std::string page;

    delimeter = "\r\n";
    this->buildHeadOfPage(page, delimeter, "autoIndex", path);
    page.append("<body>").append(delimeter);
    page.append("    <h1>Index of ").append(path).append("</h1>").append(delimeter);
    page.append("    <ul>").append(delimeter);
    DIR *dir;
    struct dirent *entry;
    if ((dir = opendir(path.c_str())) != NULL)
    {
        while ((entry = readdir(dir)) != NULL) {
            std::string item = entry->d_name;
            if (entry->d_type == DT_DIR) {
                page.append("            <li><a href=\"").append(item).append("/\">").append(item).append("/</a></li>").append(delimeter);
            } else {
                page.append("            <li><a href=\"").append(item).append("\">").append(item).append("/</a></li>").append(delimeter);
            }
        }
        closedir(dir);
    }
    page.append("    </ul>").append(delimeter);
    page.append("</body>").append(delimeter);
    page.append("</html>").append(delimeter);
    path.clear();
    path = page;
//    std::cout << "fim    | buildAutoindex" << std::endl;
}

void    Client::buildHeadOfPage(std::string & page, const std::string & delimeter, std::string status, const std::string & path)
{
    page.append("<!DOCTYPE html>").append(delimeter);
    page.append("<html lang=\"pt-br\">").append(delimeter);
    page.append("<head>").append(delimeter);
    page.append("    <meta charset=\"UTF-8\">").append(delimeter);
    page.append("    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">").append(delimeter);
    if (status.compare(0, status.size(), "autoIndex") == 0)
        page.append("    <title>Index of ").append(path).append("</title>").append(delimeter);
    else if (status.compare(0, status.size(), "404") == 0)
        page.append("    <title>Erro ").append(status).append(" - Default Erro Interno do Servidor</title>").append(delimeter);
    else if (status.compare(0, status.size(), "405") == 0)
        page.append("    <title>Erro ").append(status).append(" - Default Erro Interno do Servidor</title>").append(delimeter);
    else if (status.compare(0, status.size(), "408") == 0)
        page.append("    <title>Erro ").append(status).append(" - Default Erro Interno do Servidor</title>").append(delimeter);
    else if (status.compare(0, status.size(), "500") == 0)
        page.append("    <title>Erro ").append(status).append(" - Default Erro Interno do Servidor</title>").append(delimeter);
    page.append("</head>").append(delimeter);
}

void    Client::buildDefaultErrorPage(std::string & page, const std::string & errorCode)
{
//    std::cout << "Início | buildDefaultErrorPage: " << page << " and statusError: " << errorCode << std::endl;
    std::string delimeter;

    delimeter = "\r\n";
    this->buildHeadOfPage(page, delimeter, errorCode, "");
    page.append("<body>").append(delimeter);
    page.append("    <div>").append(delimeter);
    page.append("        <h1>Erro ").append(errorCode).\
                                append(" - Default</h1>").append(delimeter);
    if (errorCode.compare(0, errorCode.size(), "404") == 0)
    {
        page.append("        <p>Página não encontrada</p>").append(delimeter);
        page.append("        <p>Desculpe, a página que você está procurando").\
        append(" pode ter sido removida, renomeada ou estar temporariamente").\
        append(" indisponível.</p>").append(delimeter);
    }
    else if (errorCode.compare(0, errorCode.size(), "405") == 0)
    {
        page.append("        <p>Método não permitido</p>").append(delimeter);
        page.append("        <p>Desculpe, a página que você está procurando").\
        append(" não permite o médoto solicitado.").append(delimeter);
    }
    else if (errorCode.compare(0, errorCode.size(), "408") == 0)
    {
        page.append("        <p>Timeout</p>").append(delimeter);
        page.append("        <p>Desculpe, mas a requisição excedeu o tempo").\
        append(" permitido.</p>").append(delimeter);
    }
    else if (errorCode.compare(0, errorCode.size(), "500") == 0)
    {
        page.append("        <p>Erro Interno do Servidor</p>").append(delimeter);
        page.append("        <p>O servidor encontrou um erro interno ou ").\
        append("configuração incorreta que o impediu de atender à ").\
        append("solicitação.</p>").append(delimeter);
    }
    else
    {
        page.append("        <p>Erro Não identificado no Servidor</p>").\
        append(delimeter);
        page.append("        <p>O servidor encontrou um erro não ").\
        append("identificado que o impediu de atender à solicitação.</p>").\
        append(delimeter);
    }
    page.append("    </div>").append(delimeter);
    page.append("</body>").append(delimeter);
    page.append("</html>").append(delimeter);
    page.append("keyPage").append(delimeter);
//    std::cout << "final  | buildDefaultErrorPage: " << page << std::endl;
}
