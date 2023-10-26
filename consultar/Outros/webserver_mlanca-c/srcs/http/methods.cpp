#include "http/methods.hpp"

#include "cgi/CgiHandler.hpp"
#include "config/Options.hpp"
#include "http/Handler.hpp"
#include "http/Request.hpp"
#include "http/Response.hpp"
#include "utils/MimeType.hpp"
#include "utils/smt.hpp"

#include <dirent.h>
#include <sstream>
#include <unistd.h>

namespace http {
namespace methods {

smt::shared_ptr<http::Response>
    GET(smt::shared_ptr<http::Request> const request,
        smt::shared_ptr<config::Opts> const  opts) {
    smt::shared_ptr<http::Response> resp;

    // check if file/directory is valid
    std::string file = request->routeRequest();
    if (!ft::file(file).isValid() && !ft::directory(file).isValid()) {
        return (generateErrorResponse(404, opts));
    }

    // checking if its a directory
    if (ft::file(file).isDirectory()) {
        // check for index and autoindex
        if (!std::string(opts->m_index).empty()) {
            file = opts->m_index;
            if (!ft::file(file).isValid()) {
                return (generateErrorResponse(404, opts));
            }
        }
        else if (opts->m_autoindex) {
            return (generateAutoIndexResponse(request));
        }
        else { return (generateErrorResponse(403, opts)); }
    }

    if (!opts->m_cgi_extension.empty() &&
        opts->m_cgi_extension == ft::file(file).getExtension()) {
        // checking if file was changed
        if (file != request->routeRequest()) {
            smt::shared_ptr<http::Route> route(new http::Route("/", "/"));
            request->setRoute(route);
            request->setPath(file);
        }
        return (generateCgiResponse(request, opts));
    }

    std::map<std::string, std::string> headers;
    headers["Content-Type"] = http::MimeType(file);
    std::string body = ft::file(file).read();

    // return response
    resp = smt::make_shared(new http::Response(200, headers, body));
    return (resp);
}

smt::shared_ptr<http::Response>
    POST(smt::shared_ptr<http::Request> const request,
         smt::shared_ptr<config::Opts> const  opts) {
    smt::shared_ptr<http::Response> resp;

    std::string file = request->routeRequest();

    // checking if directory of file is valid
    ft::directory fileDir = ft::file(file).getDirectory();
    if (!fileDir.isValid()) { return (generateErrorResponse(404, opts)); }

    // checking if file is crawler
    if (ft::file(request->getPath()).isCrawler()) {
        return (generateErrorResponse(404, opts));
    }

    // checking if file exists
    if (!ft::file(file).exists()) {
        std::ofstream f(file.c_str(), std::ios_base::app);
        if (!f.good()) { return (generateErrorResponse(403, opts)); }
        f << request->getBody();
        f.close();
    }
    else {
        if (!opts->m_cgi_extension.empty() &&
            opts->m_cgi_extension == ft::file(file).getExtension()) {
            return (generateCgiResponse(request, opts));
        }
        else {

            std::ofstream f(file.c_str(),
                            std::ofstream::out | std::ofstream::app);
            if (!f.good()) { return (generateErrorResponse(403, opts)); }
            f << request->getBody();
            f.close();
        }
    }

    std::map<std::string, std::string> headers;
    resp = smt::make_shared(new http::Response(201, headers));

    return (resp);
}

smt::shared_ptr<http::Response>
    DELETE(smt::shared_ptr<http::Request> const request,
           smt::shared_ptr<config::Opts> const  opts) {
    smt::shared_ptr<http::Response> resp;

    // checking if file is valid
    std::string file = request->routeRequest();
    if (ft::file(file).isDirectory()) {
        return (generateErrorResponse(403, opts));
    }
    if (!ft::file(file).isValid()) {
        return (generateErrorResponse(404, opts));
    }

    // Delete the file
    if (unlink(file.c_str()) == -1) { generateErrorResponse(403, opts); }

    // return response
    std::map<std::string, std::string> headers;
    resp = smt::make_shared(new http::Response(204, headers));
    return (resp);
}

smt::shared_ptr<http::Response>
    generateAutoIndexResponse(smt::shared_ptr<http::Request> const req) {

    smt::shared_ptr<http::Response> resp;

    // Get the list of files in the directory
    std::string routedPath = req->routeRequest();
    std::string path = req->getPath();

    std::stringstream ss;
    ss << "<html><head><title>Index of " << path << "</title></head><body>"
       << "<h1>Index of " << path << "</h1>"
       << "<ul>";

    dirent* entry;
    DIR*    dir = opendir(routedPath.c_str());
    while ((entry = readdir(dir)) != NULL) {
        std::string fileName = entry->d_name;
        if (fileName != "." && fileName != "..") {
            ss << "<li><a href=\"" << std::string(ft::directory(path))
               << fileName << "\">" << fileName << "</a></li>";
        }
    }
    closedir(dir);

    ss << "</ul>";
    ss << "</body></html>";

    // creating body
    std::string body = ss.str();

    // creating headers
    std::map<std::string, std::string> headers;
    headers["Content-Type"] = "text/html";

    resp = smt::make_shared(new http::Response(200, headers, body));
    return (resp);
}

smt::shared_ptr<http::Response>
    generateCgiResponse(smt::shared_ptr<http::Request> const request,
                        smt::shared_ptr<config::Opts> const  opts) {
    smt::shared_ptr<http::Response> resp;
    cgi::CgiHandler                 handler(request);
    std::string                     respStr = handler.run();
    try {
        resp = smt::make_shared(new http::Response(respStr));
    }
    catch (http::Response::MalformedResponseException const&) {
        return (generateErrorResponse(500, opts));
    }
    return (resp);
}

smt::shared_ptr<Response>
    generateErrorResponse(int code, smt::shared_ptr<config::Opts> const opts) {

    std::string                        body;
    std::map<std::string, std::string> header;
    std::stringstream                  ss;

    // getting custom body
    bool default_body = false;
    if (opts->m_error_pages.find(code) != opts->m_error_pages.end()) {
        std::string   filename = std::string(opts->m_error_pages[code]);
        std::ifstream file(filename.c_str());
        if (!file.good()) {
            LOG_I("Fail to open " << filename);
            default_body = true;
        }
        else {
            body = std::string((std::istreambuf_iterator<char>(file)),
                               std::istreambuf_iterator<char>());
            header["Content-Type"] = http::MimeType(filename);
        }
        file.close();
    }
    else { default_body = true; }

    // getting default body
    if (default_body) {

        // converting code to str
        ss << code;
        std::string error_code = ss.str();

        // generating message
        std::string msg =
            error_code + " - " + std::string(http::ErrorCode(code));
        body = "<!DOCTYPE html>"
               "<html>"
               "  <head>"
               "    <title>" +
               msg +
               "</title>"
               "  </head>"
               "  <body>"
               "    <h1>" +
               msg +
               "</h1>"
               "  </body>"
               "</html>";
        header["Content-Type"] = "text/html";
    }

    return (smt::shared_ptr<Response>(new Response(code, header, body)));
}

} // namespace methods

MethodType convertMethod(std::string const& methodStr) {
    if (methodStr == "GET") { return (GET); }
    if (methodStr == "HEAD") { return (HEAD); }
    if (methodStr == "POST") { return (POST); }
    if (methodStr == "PUT") { return (PUT); }
    if (methodStr == "DELETE") { return (DELETE); }
    if (methodStr == "CONNECT") { return (CONNECT); }
    if (methodStr == "OPTIONS") { return (OPTIONS); }
    if (methodStr == "TRACE") { return (TRACE); }
    if (methodStr == "PATCH") { return (PATCH); }
    if (methodStr == "INVALID") { return (INVALID); }
    return (UNKNOWN_METHOD);
}

Version convertVersion(std::string const& versionStr) {
    if (versionStr == "HTTP/1.1") { return (HTTP_1_1); }
    return (UNKNOWN_VERSION);
}

} // namespace http
