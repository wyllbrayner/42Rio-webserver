NAME = webserv

# Compiler and flags
CXX 		= g++
CXXFLAGS	= -Wall -Werror -Wextra -std=c++98

#					Source and object file paths
MAIN					=	main.cpp Utils.cpp Error.cpp

CORE_DIR				=	core/
CORE_FILES				=	Webserv.cpp Server.cpp Connection.cpp
CORE					=	$(addprefix $(CORE_DIR), $(CORE_FILES))

HTTP_DIR				=	http/
HTTP_FILES				=	Request.cpp Response.cpp Client.cpp CGI.cpp
HTTP					=	$(addprefix $(HTTP_DIR), $(HTTP_FILES))

CONFIG_DIR				=	config/
CONFIG_FILES			=	ConfigFile.cpp Location.cpp ParserServer.cpp 
#CONFIG_FILES			=	ConfigFile.cpp Utils.cpp Location.cpp ParserServer.cpp Error.cpp
CONFIG					=	$(addprefix $(CONFIG_DIR), $(CONFIG_FILES))

SRC_DIR 				=	source/
ALL_SRCS 				=	$(addprefix $(SRC_DIR), $(MAIN) $(CORE) $(HTTP) $(CONFIG))

#					###### OBJECTS ######
OBJS 					=	$(patsubst $(SRC_DIR)%.cpp, $(OBJ_DIR)%.o, $(ALL_SRCS))
OBJ_DIR 				=	obj/
ALL_OBJ_DIR				=	$(sort $(dir $(OBJS)))

#					########### Rules #############
all: $(ALL_OBJ_DIR) $(NAME)

# Create the object directory if it doesn't exist

$(ALL_OBJ_DIR):
	@mkdir -p $(ALL_OBJ_DIR)

# Compile source files into object files
$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo -n "#"

# Link object files into the final executable
$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@printf "\n$(_SUCCESS) webserv now is ready.\n"

clean:
	@rm -rf $(OBJ_DIR)
	@printf "$(YELLOW)Objects removed.\n$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@printf "$(YELLOW)Executable removed.\n$(RESET)"

re: fclean all

# ############################ COLORS #################

GREEN = \033[0;32m
YELLOW = \033[0;33m
RESET = \033[0m
_SUCCESS = ✅ $(GREEN)Successfully compiled$(RESET)

#######################################################3
.PHONY: all clean fclean re
