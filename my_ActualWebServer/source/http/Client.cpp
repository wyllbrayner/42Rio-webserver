#include "Client.hpp"

Client::Client(const Request &request, Response &response) : \
        _request(request), _response(response), _isCGI(false)
{
    this->handleHTTPMethod();
}

Client::~Client() {}

void                    Client::handleHTTPMethod(void)
{
    std::string pagePath;
    std::stringstream page;

    pagePath = this->fileRequested();
    if (_isCGI){
        CGI cgi(pagePath, this->_request);
        
        if (cgi.executeCGI()){
            this->_response.setBody(cgi.getBody());
            this->_response.createHTTPHeader2(this->getStatusCode(), "text/html; charset=utf-8");
            this->_response.send();
        }
        else {
            pagePath.clear();
            this->buildDefaultErrorPage(pagePath, "500");
            if (pagePath.find("keyPage") != std::string::npos)
            pagePath.erase(pagePath.find("keyPage"));
            this->_response.setBody(pagePath);
            this->_response.createHTTPHeader2("500 Internal Server Error", "text/html; charset=utf-8");
            this->_response.send();
        }
        return;
    }
    else
    {
        if (pagePath.compare(0, 2, "./") == 0)
        {
            std::ifstream file(pagePath.c_str());
            if (file)
            {
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
        this->_response.processFileForHTTPResponse2(page, this->getStatusCode());
        this->_response.send();
    }
    this->_statusCode.clear();
}

const std::string   &   Client::getStatusCode(void)
{
    return (this->_statusCode);
}

std::string             Client::fileRequested(void)
{
    std::string fileRequested;
    size_t      i;

    this->selectContent(fileRequested, i);
    if (this->_request.getMethod().compare(0, 3, "GET") == 0)
    {
        if (this->_request.getLocation().find("cgi-bin") != std::string::npos)
            this->_isCGI = true;
        else
            this->buildGetfileRequested(fileRequested);
    }
    else if (this->_request.getMethod().compare(0, 6, "DELETE") == 0)
        this->buildDeletefileRequested(fileRequested);
    else if (this->_request.getMethod().compare(0, 4, "POST") == 0)
        this->_isCGI = true;
    if (fileRequested.empty() || fileRequested.compare(0, 5, "Error") == 0 || \
        (fileRequested.find("keyPage") == std::string::npos))
        this->buildErrorfileRequested(fileRequested, i);
    if (fileRequested.find("keyPage") != std::string::npos)
        fileRequested.erase(fileRequested.find("keyPage"));
    return (fileRequested);
}

void                    Client::selectContent(std::string & fileRequested, \
                        size_t & i)
{
    std::vector<std::string>::iterator  it00;
    std::vector<std::string>            tmpVec00;
    std::vector<std::string>            tmpVec01;
    size_t                              j;
    size_t                              k;

    i = 0;
    while (i < this->_request.getServerConf().getLocation().size())
    {
        if (this->_request.getServerConf().getLocation()[i].getPath() == \
            this->_request.getLocation())
        {
            tmpVec00 = \
                this->_request.getServerConf().getLocation()[i].getMethods();
            j = 0;
            while ((j < tmpVec00.size()) && \
                (tmpVec00[j].compare(this->_request.getMethod()) != 0))
                j++;
            if ((j < tmpVec00.size()))
            {
                tmpVec01 = this->_request.getServerConf().getLocation()[i].getIndex();
                k = 0;
                while (k < tmpVec01.size() && (tmpVec01[k].compare(this->_request.getRequestedInf()) != 0))
                    k++;
                if (k < tmpVec01.size())
                    fileRequested.append(".").append(this->_request.getServerConf().getLocation()[i].getReturn()).append(this->_request.getRequestedInf());
                else
                    fileRequested.append("Error404");
                if (this->_request.getServerConf().getLocation()[i].getAutoIndex())
                    fileRequested.append("autoindex");
                this->_statusCode = "200 OK";
                break ;
            }
            else
            {
                fileRequested.append("Error405");
                break ;
            }
        }
		i++;
	}
    if (i == this->_request.getServerConf().getLocation().size())
        fileRequested.append("Error404");
}

void                    Client::buildGetfileRequested(std::string & \
                        fileRequested)
{
    if (this->_request.getServerConf().getIsServerDefault() && \
                    fileRequested.compare(0, 5, "Error") != 0)
        this->buildDefaultPage(fileRequested);
    else if (fileRequested.find("autoindex") != std::string::npos)
    {
        fileRequested.erase(fileRequested.find("autoindex"));
        if (fileRequested.compare(0, 5, "Error") != 0)
        {
            if (Utils::getTypePath(fileRequested) != 1)
                this->buildAutoindexPage(\
                        fileRequested.erase(fileRequested.rfind("/") + 1));
        }
    }
}

void                    Client::buildDeletefileRequested(std::string & \
                        fileRequested)
{
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
}

void                    Client::buildErrorfileRequested(std::string & \
                        fileRequested, const size_t & i)
{
   	if (fileRequested.empty() || fileRequested.compare(0, 5, "Error") == 0)
    {
        if (fileRequested.empty())
            this->searchErrorFile(fileRequested, "404");
        else
            this->searchErrorFile(fileRequested, fileRequested.substr(5, 3));
    }
    if ((fileRequested.find("keyPage") == std::string::npos) && \
        (Utils::getTypePath(fileRequested) != 1) && \
        (!this->_request.getServerConf().getLocation()[i].getAutoIndex()))
	{
        if (fileRequested.compare(3, 7, "Default") != 0)
        {
            this->searchErrorFile(fileRequested, "500");
            if (Utils::getTypePath(fileRequested) != 1)
                this->searchErrorFile(fileRequested, "500");
        }
    }
}

void                    Client::buildHeadOfPage(std::string & page, \
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
    else if ((status.find("POST") != std::string::npos) || \
        (status.find("GET") != std::string::npos))
        page.append("    <title>").append(status).append("</title>").append(delimeter);
    page.append("</head>").append(delimeter);
}

void                    Client::buildDefaultPage(std::string & page)
{
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
}

void                    Client::buildAutoindexPage(std::string & path)
{
    std::string     delimeter;
    std::string     page;
    DIR             *dir;
    struct dirent   *entry;

    delimeter = "\r\n";
    this->buildHeadOfPage(page, delimeter, "autoIndex", path);
    page.append("<body>").append(delimeter);
    page.append("    <h1>Index of ").append(path).append("</h1>").append(delimeter);
    page.append("    <ul>").append(delimeter);
    dir = opendir(path.c_str());
    if (dir != NULL)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            std::string item = entry->d_name;
            if (entry->d_type == DT_DIR)
            {
                page.append("            <li><a href=\"")\
                .append(item).append("/\">").append(item)\
                .append("/</a></li>").append(delimeter);
            }
            else
                page.append("            <li><a href=\"")\
                .append(item).append("\">").append(item)\
                .append("/</a></li>").append(delimeter);
        }
        closedir(dir);
    }
    page.append("    </ul>").append(delimeter);
    page.append("</body>").append(delimeter);
    page.append("</html>").append(delimeter);
    path.clear();
    path = page;
}

void                    Client::buildDeleteFile(const std::string & path, \
                        const std::string & idValue)
{
	std::ifstream	ifs;
    std::ofstream   ofs;
	std::string		line;
	std::string		page;

	ifs.open(path.c_str());
	if (ifs.is_open())
	{
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
	}
    if (!page.empty())
    {
        ifs.open(path.c_str());
        if (ifs.is_open())
        {
            ofs.open(path.c_str(), std::ios::out | std::ios::trunc);
            if (ofs.is_open())
            {
                ofs << page;
                ofs.close();
            }
            ifs.close();
        }
    }
}

void                    Client::searchErrorFile(std::string & fileRequested, \
                        std::string errorCode)
{
    size_t  i;
    
    i = 0;
    while (i < this->_request.getServerConf().getErrorPage().size())
    {
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
}

void                    Client::buildDefaultErrorPage(std::string & page, \
                        const std::string & errorCode)
{
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
}