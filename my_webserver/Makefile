SRCS		= $(wildcard *.cpp)
OBJS		= $(SRCS:.cpp=.o)
INCLUDE		= $(wildcard *.hpp)
NAME		= webserver
FLAG		= -Wall -Wextra -Werror -std=c++98
COMP		=  c++

all: $(NAME)

%.o:	%.cpp
	$(COMP) $(FLAG) $< -c -o $@

$(NAME): $(OBJS) $(INCLUDES)
	$(COMP) $(FLAG) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re