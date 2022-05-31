NAME	= ircserv

SRC		=	srcs/main.cpp\
 			srcs/User.cpp\
 			srcs/Server.cpp\
 			srcs/Utils.cpp\
 			srcs/Logger.cpp\
 			srcs/CommandHandle.cpp\
 			srcs/Command.cpp\
 			srcs/Poll.cpp

CXX		= c++

CPPFLAGS	= -g# -std=c++98 -Wall -Wextra -Werror

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