NAME			= webserver
COMP			= c++
FLAG			= -Wall -Werror -Wextra -std=c++98
RM				= rm -rf
YELLOW			= \033[33m
GREEN			= \033[32m
RESET			= \033[0m

OBJDIR			= ./obj/
OBJS			= $(OBJSMAIN) $(OBJSCORE) $(OBJSHTTP) $(OBJSCONFIG)

SRCSDIR			= ./src/
MAIN			= main.cpp Utils.cpp Error.cpp
OBJSMAIN		= $(addprefix $(OBJDIR), $(MAIN:.cpp=.o))

CORE_DIR		= core/
CORE_FILES		= Webserv.cpp Server.cpp Connection.cpp
OBJSCORE		= $(addprefix $(OBJDIR)$(CORE_DIR), $(CORE_FILES:.cpp=.o))

HTTP_DIR		= http/
HTTP_FILES		= Request.cpp Response.cpp Client.cpp CGI.cpp
OBJSHTTP		= $(addprefix $(OBJDIR)$(HTTP_DIR), $(HTTP_FILES:.cpp=.o))

CONFIG_DIR		= config/
CONFIG_FILES	= ConfigFile.cpp Location.cpp ParserServer.cpp
OBJSCONFIG		= $(addprefix $(OBJDIR)$(CONFIG_DIR), $(CONFIG_FILES:.cpp=.o))


all: $(NAME)

$(OBJDIR)%.o: $(SRCSDIR)%.cpp
	@mkdir -p $(OBJDIR)$(CORE_DIR) $(OBJDIR)$(HTTP_DIR) $(OBJDIR)$(CONFIG_DIR)
	@$(COMP) $(FLAG) -c $< -o $@
	@echo -n "#"

$(NAME): $(OBJS)
	@$(COMP) $(FLAG) -o $(NAME) $(OBJS)
	@echo "\n\$(GREEN) 💯 | $(NAME) created.$(RESET)"

clean:
	@$(RM) $(OBJDIR)
	@echo "$(YELLOW) 🧹 | $(NAME) cleaned.$(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(YELLOW) 🌪️ | $(NAME) all cleaned.$(RESET)"

re: fclean all

.PHONY: all $(NAME) clean fclean re
