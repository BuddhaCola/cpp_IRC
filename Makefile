NAME	= ircserv

SRC		=	srcs/main.cpp

CXX		= c++

CPPFLAGS	= -g -Wall -Wextra -Werror

OBJ		= $(SRC:.cpp=.o)

$(NAME): $(OBJ)
	$(CXX) $(CPPFLAGS) $(OBJ) -o $(NAME)

all: $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean