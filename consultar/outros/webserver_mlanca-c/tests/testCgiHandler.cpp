#include "cgi/Argv.hpp"
#include "cgi/CgiHandler.hpp"
#include "cgi/Envp.hpp"
#include "http/Request.hpp"
#include "utils/ft_array.hpp"
#include "utils/Logger.hpp"

#include <gtest/gtest.h>

TEST(testCgiTypes, testConvertType) {
    EXPECT_EQ(cgi::CgiHandler::convertCgiExtension(".py"), cgi::Py);
    EXPECT_EQ(cgi::CgiHandler::convertCgiExtension(".php"), cgi::Php);
    EXPECT_EQ(cgi::CgiHandler::convertCgiExtension(".cgi"), cgi::Cgi);
    EXPECT_EQ(cgi::CgiHandler::convertCgiExtension(".other"), cgi::Unknown);
    EXPECT_EQ(cgi::CgiHandler::convertCgiExtension("other"), cgi::Unknown);
}

class testCgi : public ::testing::Test {
    protected:

        void SetUp(void) {
            std::string reqStr = "GET /python/test.py?name=sotto HTTP/1.1\r\n"
                                 "Host: x\r\n\r\n";
            m_req = smt::make_shared(new http::Request(reqStr));
            smt::shared_ptr<http::Route> route(
                new http::Route("/python/", "./websites/cgi-bin/"));
            m_req->setRoute(route);
        }

        smt::shared_ptr<http::Request> m_req;
};

TEST_F(testCgi, testArgv) {
    cgi::CgiHandler cgi(m_req);
    char**          argv = cgi.getArgv();
    EXPECT_STREQ(argv[0], "test.py");
    EXPECT_STREQ(argv[1], NULL);
}

TEST_F(testCgi, testEnvp) {
    cgi::CgiHandler cgi(m_req);
    char**          envp = cgi.getEnvp();
    EXPECT_STREQ(envp[0], "REQUEST_METHOD=GET");
    EXPECT_STREQ(envp[1], "PATH_INFO=test.py");
    EXPECT_STREQ(envp[2], "DOCUMENT_ROOT=./websites/cgi-bin/");
    EXPECT_STREQ(envp[3], "QUERY_STRING=name=sotto");
    EXPECT_STREQ(envp[4], "CONTENT_LENGTH=");
    EXPECT_STREQ(envp[5], "CONTENT_TYPE=");
    EXPECT_STREQ(envp[6], NULL);
}

TEST_F(testCgi, testingHelloWorld) {
    // creating file
    system("touch ./websites/cgi-bin/test.py");
    system("chmod +x ./websites/cgi-bin/test.py");
    system("echo '#!/usr/bin/env python\n"
           "print(\"Hello World!\")' > "
           "./websites/cgi-bin/test.py");
    cgi::CgiHandler cgi(m_req);
    std::string     res = cgi.run();
    EXPECT_EQ("Hello World!\n", res);
    system("rm ./websites/cgi-bin/test.py");
}

TEST_F(testCgi, testingQuery) {
    system("touch ./websites/cgi-bin/test.py");
    system("chmod +x ./websites/cgi-bin/test.py");
    system("echo '#!/usr/bin/env python\n"
           "import cgi\n"
           "form = cgi.FieldStorage()\n"
           "print(form.getvalue(\"name\"))' > "
           "./websites/cgi-bin/test.py");

    cgi::CgiHandler cgi(m_req);
    std::string     res = cgi.run();
    EXPECT_EQ("sotto\n", res);
    system("rm ./websites/cgi-bin/test.py");
}