# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mabriand <mabriand@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/29 16:52:41 by vmoreau           #+#    #+#              #
#    Updated: 2022/02/16 12:27:29 by mabriand         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	webserv

#################################1## COLORS ####################################
#--------------------------------RESET-#
NC 		=	\033[0m

#------------------------------REGULAR-#
BLACK	=	\033[0;30m
RED		=	\033[0;31m
GREEN	=	\033[32m
YELLOW	=	\033[33;33m
BLUE	=	\033[0;34m
PURPLE	=	\033[35m
CYAN	=	\033[1;36m
WHITE	=	\033[0;37m

#################################### PATHS #####################################
CONF	=	srcs/conf/
REQ		=	srcs/request/
RESP	=	srcs/response/
SERV	=	srcs/server/
CGI		=	srcs/cgi/

#################################### SOURCES ###################################
SRCS	+=	srcs/main.cpp

#------------------------CONFIGURATION-#
SRCS	+=	$(CONF)confpars.cpp			$(CONF)serv_block.cpp\
			$(CONF)loc_block.cpp

#------------------------------REQUEST-#
SRCS	+=	$(REQ)Request.cpp			$(REQ)Request_referer.cpp\
			$(REQ)Request_url.cpp		$(REQ)Request_setter.cpp

#-----------------------------RESPONSE-#
SRCS	+=	$(RESP)Response.cpp			$(RESP)Response_autoindex.cpp\
			$(RESP)Response_prepare.cpp

#-------------------------------SERVER-#
SRCS	+=	$(SERV)Server.cpp

#----------------------------------CGI-#
SRCS	+=	$(CGI)CgiProcess.cpp

#################################### BASICS ####################################
CFLAGS	=	-Wall -Werror -Wextra -std=c++98

CFLAGS	+=	-g3 -fsanitize=address

CC		=	clang++

INC		=	incs/

HEADER	=	$(INC)

OBJ		=	$(SRCS:.cpp=.o)

#################################### RULES #####################################
all : $(NAME)

$(NAME) : echoCW $(OBJ) echoOK echoCS
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.cpp $(HEADER)
	$(CC) -c $(CFLAGS) -I $(INC) $< -o $@
	printf "$(GREEN)██"

clean :	echoCLEAN
	$(RM) $(OBJ)

fclean : clean echoFCLEAN
	$(RM) $(NAME)

re : fclean all

test : $(NAME)
	./webserv

##################################### ECHO #####################################
echoCW:
	echo "$(YELLOW)===> Compiling Webserv$(NC)"
echoOK:
	echo "$(GREEN)OK$(NC)"
echoCS :
	echo "$(GREEN)===> Compilation Success$(NC)"
echoCLEAN :
	echo "$(PURPLE)===> Cleanning OBJ$(NC)"
echoFCLEAN :
	echo "$(PURPLE)===> Cleanning Execs$(NC)"



.PHONY : all clean fclean re

.SILENT :
