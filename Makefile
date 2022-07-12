NAME	= ircserv

SRC		=	srcs/main.cpp\
 			srcs/User.cpp\
 			srcs/Server.cpp\
 			srcs/Utils.cpp\
 			srcs/Logger.cpp\
 			srcs/Channel.cpp\
 			srcs/CommandHandle.cpp\
 			srcs/Command.cpp\
 			srcs/Poll.cpp\
 			srcs/SendError.cpp\
 			srcs/SendReply.cpp\
 			srcs/PingClient.cpp\
			srcs/methodHandlers/handlePrivateMessage.cpp\
			srcs/methodHandlers/handlePassword.cpp\
			srcs/methodHandlers/handleSetNick.cpp\
			srcs/methodHandlers/handleUser.cpp\
			srcs/methodHandlers/handlePing.cpp\
			srcs/methodHandlers/handlePong.cpp\
			srcs/methodHandlers/handleJoin.cpp\
			srcs/methodHandlers/handleQuit.cpp\
			srcs/methodHandlers/handleOper.cpp\
			srcs/methodHandlers/handleKick.cpp\
			srcs/methodHandlers/handleKill.cpp\
			srcs/methodHandlers/handlePart.cpp\
			srcs/methodHandlers/handleWho.cpp\
			srcs/methodHandlers/handleNoticeMessage.cpp\
			srcs/Bot/HandleBotMessage.cpp


CXX		= c++

CPPFLAGS	= -g -std=c++98 -Wall -Wextra -Werror

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