NAME = webserv

CXX = clang++

CXXFLAGS = -Wall -Werror -Wextra -std=c++98  -fsanitize=address

CONF_PATH = conf/

CONF_FILE = ServerInfo.cpp	parse_servers.cpp

SOCK_PATH = socket/

SOCK_FILE = create_listening_sockets.cpp

LINX_PATH = epoll_linux/

LINX_FILE = epoll_operation.cpp

MACS_PATH = kqueue_macos/

MACS_FILE = kqueue_operation.cpp

HTTP_PATH = http/

HTTP_FILE = deal_with_request.cpp	HTTPRequest.cpp	HTTPResponse.cpp	Autoindex.cpp	UploadFile.cpp\
Cgi.cpp

SRCS_PATH = srcs/

SRCS_FILE = main.cpp	utils.cpp	start_server.cpp	close_sockets.cpp	ClientInfo.cpp

SRCS_FILE += $(addprefix $(CONF_PATH), $(CONF_FILE))

SRCS_FILE += $(addprefix $(SOCK_PATH), $(SOCK_FILE))

SRCS_FILE += $(addprefix $(LINX_PATH), $(LINX_FILE))

SRCS_FILE += $(addprefix $(MACS_PATH), $(MACS_FILE))

SRCS_FILE += $(addprefix $(HTTP_PATH), $(HTTP_FILE))

SRCS = $(addprefix $(SRCS_PATH), $(SRCS_FILE))

OBJS = $(SRCS:.cpp=.o)

HEADER_FILE = webserv.hpp	ServerInfo.hpp	exception.hpp	utils.hpp	conf.hpp\
socket.hpp	connect.hpp	MACROS.hpp	ClientInfo.hpp	http.hpp\
HTTPRequest.hpp	HTTPResponse.hpp	Autoindex.hpp	UploadFile.hpp\
Cgi.hpp

HEADER_PATH = includes/

HEADER = $(addprefix $(HEADER_PATH), $(HEADER_FILE))

all : $(NAME)
	echo "Compile OK"

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) -I $(HEADER_PATH) $(OBJS) -o $(NAME)

%.o : %.cpp $(HEADER)
	$(CXX) $(CXXFLAGS) -I $(HEADER_PATH) -c $< -o $@

clean :
	rm -rf $(OBJS)
	echo "clean OK"

fclean :
	rm -rf $(OBJS) $(NAME)
	echo "fclean OK"

re : fclean all

.PHONY: all clean fclean re

.SILENT :
