#include "http/Handler.hpp"

#include "cgi/CgiHandler.hpp"
#include "config/Options.hpp"
#include "http/methods.hpp"
#include "http/Request.hpp"
#include "http/Response.hpp"
#include "utils/Logger.hpp"
#include "utils/MimeType.hpp"
#include "utils/Route.hpp"

namespace http {

smt::shared_ptr<Response>
    processRequest(int status, smt::shared_ptr<Request> const request,
                   smt::shared_ptr<config::Opts> const opts) {

    if (status) { return (methods::generateErrorResponse(status, opts)); }

    // checking max body size
    if (request->getBody().size() > opts->m_max_body_size) {
        LOG_E("Body size too large");
        return (methods::generateErrorResponse(400, opts));
    }

    // check if method is allowed
    if (opts->m_allowed_methods.find(request->getMethod()) ==
        opts->m_allowed_methods.end()) {
        LOG_I("Method not allowed");
        return (methods::generateErrorResponse(405, opts));
    }

    // getting method
    MethodType method = convertMethod(request->getMethod());
    switch (method) {
        case GET: return (methods::GET(request, opts));
        case POST: return (methods::POST(request, opts));
        case DELETE: return (methods::DELETE(request, opts));
        default: return (methods::generateErrorResponse(501, opts));
    }
}

} // namespace http