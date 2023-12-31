#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string.h>
# include <iostream>

class   Response {
    public:
        Response();
        ~Response();

        //std::string const&  _method;
        std::string httpMessage;
};

#endif