#include "Client.hpp"

Client::Client(const Request &request, Response &response) : \
        _request(request), _response(response), _isCGI(false)
{
    this->handleHTTPMethod();
}

Client::~Client() {}

void    Client::handleHTTPMethod(void)
{
    std::string pagePath;
    std::ostringstream page;

    pagePath = this->fileRequested();
    if (_isCGI){
        CGI cgi(pagePath, this->_request);
        this->_response.setBody(cgi.getBody());
        this->_response.createHTTPHeader(200, "Content-Type: text/html; charset=utf-8", cgi.getBody().size());
        this->_response.send();
        return;
    }
    if (pagePath.compare(0, 2, "./") == 0)
    {
        std::ifstream file(pagePath.c_str());
        if (file)
        {
            // Here execute	methods or CGI
            page << file.rdbuf();
            file.close();
        }
        else
        {
            pagePath.clear();
            this->buildDefaultErrorPage(pagePath, "404");
            page << pagePath;
        }
    }
    else
        page << pagePath;

    page.flush();
    std::string text;
    int lenPage = page.str().size();
    text.append("HTTP/1.1 ").append(this->getStatusCode()).append("\r\n");
    text.append("Content-Type: text/html\r\n");
    std::string content_len;
    std::stringstream sstream;
    sstream << lenPage;
    content_len.append("Content-Length: ").append(sstream.str());
    content_len += "\r\n\n";
    text.append(content_len);
    text.append(page.str());
    this->_response.httpMessage = text;
    this->_statusCode.clear();
}

const std::string & Client::getStatusCode(void)
{
    return this->_statusCode;
}

std::string Client::fileRequested(void)
{
//    std::cout << "inicio | fileRequested" << std::endl;
    std::string fileRequested;
    size_t      i;

    this->selectContent(fileRequested, i);
    if (this->_request.getMethod().compare(0, 3, "GET") == 0)
    {
        if (this->_request.getLocation().find("cgi-bin") != std::string::npos)
        {
            std::cout << "MÈTODO " << this->_request.getMethod() << " COM CGI" << std::endl;
            this->_isCGI = true;
        }
        else
        {
            std::cout << "MÈTODO " << this->_request.getMethod() << " SEM CGI" << std::endl;
            this->buildGetfileRequested(fileRequested);
        }
    }
    else if (this->_request.getMethod().compare(0, 6, "DELETE") == 0)
    {
        std::cout << "MÈTODO " << this->_request.getMethod() << std::endl;
        this->buildDeletefileRequested(fileRequested);
    }
    else if (this->_request.getMethod().compare(0, 4, "POST") == 0)
    {
        this->_isCGI = true;
        std::cout << "MÈTODO " << this->_request.getMethod() << " COM CGI" << std::endl;
    }
    if (fileRequested.empty() || fileRequested.compare(0, 5, "Error") == 0 || \
        (fileRequested.find("keyPage") == std::string::npos))
    {
//        std::cout << "Chama a página de erro!!!!!!" << std::endl;
        this->buildErrorfileRequested(fileRequested, i);
    }
//    std::cout << "fileRequested is keyPage?" << std::endl;
    if (fileRequested.find("keyPage") != std::string::npos)
        fileRequested.erase(fileRequested.find("keyPage"));
//    std::cout << "final  | fileRequested: " << fileRequested << std::endl;
//    std::cout << "final  | fileRequested code: " << this->_code << std::endl;
	return (fileRequested);
}

void        Client::selectContent(std::string & fileRequested, size_t & i)
{
//    std::cout << "inicio | selectContent | fileRequested: " << fileRequested << std::endl;
    std::vector<std::string>::iterator  it00;
    std::vector<std::string>            tmpVec00;
    std::vector<std::string>            tmpVec01;
    size_t                              j;
    size_t                              k;

	i = 0;
	while (i < this->_request.getServerConf().getLocation().size())
	{
//        std::cout << "Em Location n:\t" << (i + 1) << " Path: " << this->_request.getServerConf().getLocation()[i].getPath() << std::endl;
        if (this->_request.getServerConf().getLocation()[i].getPath() == \
            this->_request.getLocation())
        {
            tmpVec00 = this->_request.getServerConf().getLocation()[i].getMethods();
            j = 0;
            while ((j < tmpVec00.size()) && \
                (tmpVec00[j].compare(this->_request.getMethod()) != 0))
            {
//                std::cout << "Método[" << j << "]: |" << tmpVec00[j] << "|" << std::endl;
                j++;
            }
            if ((j < tmpVec00.size()))
            {
//                std::cout << "Méthodo " << tmpVec00[j] << "     autorizado!!!" << std::endl;
                tmpVec01 = this->_request.getServerConf().getLocation()[i].getIndex();
                k = 0;
                while (k < tmpVec01.size() && (tmpVec01[k].compare(this->_request.getRequestedInf()) != 0))
                {
//                    std::cout << "índice[" << k << "]: |" << tmpVec01[k] << "|" << std::endl;
                    k++;
                }
                if (k < tmpVec01.size())
                {
//                    std::cout << "Achou RequestedInf(): " << this->_request.getRequestedInf() << std::endl;
                    fileRequested.append(".").append(this->_request.getServerConf().getLocation()[i].getReturn()).append(this->_request.getRequestedInf());
                }
                else
                {
//                    std::cout << "RequestedInf: " << this->_request.getRequestedInf() << " não identificado!" << std::endl;
                    fileRequested.append("Error404");
                }
                if (this->_request.getServerConf().getLocation()[i].getAutoIndex())
                {
//                    std::cout << "Autoindex" << std::endl;
                    fileRequested.append("autoindex");
                }
                this->_statusCode = "200 OK";
                break ;
            }
            else
            {
                fileRequested.append("Error405");
//                std::cout << "Méthodo não autorizado!!!" << std::endl;
                break ;
            }
        }
		i++;
	}
    if (i == this->_request.getServerConf().getLocation().size())
        fileRequested.append("Error404");
//    std::cout << "Fim    | selectContent | fileRequested: " << fileRequested << std::endl;
}

void    Client::buildGetfileRequested(std::string & fileRequested)
{
//    std::cout << "inicio | buildGetfileRequested" << std::endl;
//    std::cout << "fileRequested: " << fileRequested << std::endl;
    if (this->_request.getServerConf().getIsServerDefault() && \
                    fileRequested.compare(0, 5, "Error") != 0)
    {
//        std::cout << "configFile é Default " << std::endl;
        this->buildDefaultPage(fileRequested);
    }
    else if (fileRequested.find("autoindex") != std::string::npos)
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
    }
//    std::cout << "fim    | buildGetfileRequested" << std::endl;
}

void    Client::buildDeletefileRequested(std::string & fileRequested)
{
//    std::cout << "inicio | buildDeletefileRequested" << std::endl;
    if ((this->_request.getMethod().compare(0, 6, "DELETE") == 0) && \
        (this->_request.getMapQueryString().size() > 0) && \
        !fileRequested.empty() && (fileRequested.compare(0, 5, "Error") != 0))
    {
        std::map<std::string, std::string> tmpMap;
        std::map<std::string, std::string>::iterator itMap;
        tmpMap = this->_request.getMapQueryString();
        itMap = tmpMap.begin();
        this->buildDeleteFile(fileRequested, itMap->second);
    }
//    std::cout << "Fim    | buildDeletefileRequested" << std::endl;
}

void    Client::buildErrorfileRequested(std::string & fileRequested, const size_t & i)
{
//    std::cout << "Início | buildErrorfileRequested" << std::endl;
   	if (fileRequested.empty() || fileRequested.compare(0, 5, "Error") == 0)
    {
//        std::cout << "fileRequested is empty or Error" << std::endl;
        if (fileRequested.empty())
            this->searchErrorFile(fileRequested, "404");
        else
            this->searchErrorFile(fileRequested, fileRequested.substr(5, 3));
    }
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
    }
//    std::cout << "Fim    | buildErrorfileRequested" << std::endl;
}

void    Client::buildHeadOfPage(std::string & page, \
        const std::string & delimeter, std::string status, \
        const std::string & path)
{
    std::string msgTagAi;
    std::string msgTagEr;
    std::string msgTitle;

    msgTagAi = "    <title>Index of ";
    msgTagEr = "    <title>Erro ";
    msgTitle = " - Default Erro Interno do Servidor</title>";
    page.append("<!DOCTYPE html>").append(delimeter);
    page.append("<html lang=\"pt-br\">").append(delimeter);
    page.append("<head>").append(delimeter);
    page.append("    <meta charset=\"UTF-8\">").append(delimeter);
    page.append("    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">").append(delimeter);
    if (status.compare(0, status.size(), "autoIndex") == 0)
        page.append(msgTagAi).append(path).append("</title>").append(delimeter);
    else if (status.compare(0, status.size(), "DefaultPage") == 0)
        page.append("    <title>DefaultPage</title>").append(delimeter);
    else if (status.compare(0, status.size(), "404") == 0)
        page.append(msgTagEr).append(status).append(msgTitle).append(delimeter);
    else if (status.compare(0, status.size(), "405") == 0)
        page.append(msgTagEr).append(status).append(msgTitle).append(delimeter);
    else if (status.compare(0, status.size(), "408") == 0)
        page.append(msgTagEr).append(status).append(msgTitle).append(delimeter);
    else if (status.compare(0, status.size(), "500") == 0)
        page.append(msgTagEr).append(status).append(msgTitle).append(delimeter);
    page.append("</head>").append(delimeter);
}

void    Client::buildDefaultPage(std::string & page)
{
//    std::cout << "Início | buildDefaultPage: " << page << std::endl;
    std::string delimeter;

    delimeter = "\r\n";
    page.clear();
    this->buildHeadOfPage(page, delimeter, "DefaultPage", "");
    page.append("<body>").append(delimeter);
    page.append("    <h1>Default Example Page</h1>").append(delimeter);
    page.append("    <p>Esta é a página inicial do servidor padrão ")\
            .append(" deste webserver</p>").append(delimeter);
    page.append("    <p>Obrigado pela visita</p>").append(delimeter);
    page.append("</body>").append(delimeter);
    page.append("</html>").append(delimeter);
    page.append("keyPage").append(delimeter);
//    std::cout << "Fim    | buildDefaultPage: " << page << std::endl;
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

void    Client::searchErrorFile(std::string & fileRequested, std::string errorCode)
{
//    std::cout << "inicio | searchErrorFile " << fileRequested << " and code: " << errorCode << std::endl;
    size_t  i;
    
    i = 0;
    while (i < this->_request.getServerConf().getErrorPage().size())
    {
//        std::cout << "errorPage[" << i << "]: " << this->_request.getServerConf().getErrorPage()[i] << std::endl;
        if (this->_request.getServerConf().getErrorPage()[i].find(errorCode) != \
            std::string::npos)
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
    this->_statusCode.clear();
    if (errorCode.compare(0, 3, "404") == 0)
        this->_statusCode.append(errorCode).append(" Not Found");
    else if (errorCode.compare(0, 3, "405") == 0)
        this->_statusCode.append(errorCode).append(" Method Not Allowed");
    else if (errorCode.compare(0, 3, "408") == 0)
        this->_statusCode.append(errorCode).append(" Request Timeout");
    else if (errorCode.compare(0, 3, "500") == 0)
        this->_statusCode.append(errorCode).append(" Internal Server Error");
//    std::cout << "final  | searchErrorFile " << fileRequested << " and statusCode: " << this->_statusCode << std::endl;
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