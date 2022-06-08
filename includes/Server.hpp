#ifndef MY_IRC_SERVER_HPP
#define MY_IRC_SERVER_HPP

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <iostream>

#include "allAnswers.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include "my_irc.hpp"
#include "Logger.hpp"
#include <sys/select.h>
#include <poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>

#define MAX_USERS 		1024
#define POLL_TIMEOUT 	7000
#define TIMEOUT 		15

class Server {
public:
	Server(int, std::string);
	void 						createFdList(int);
	User * 						checkFdUser(int i);
	void 						readFromBuffer(int i);
	void 						pollDefault(int listen_sock);
	void 						sendErrorToUser(Command const &);
	void 						ServerMessageToUser(Command const &command);
	int 						creatListenSocket(int);
	void						mainLoop(int);
	std::string					getPassword() const;
	int 						getPort() const;
	void 						pingClient();
	Server & operator= (const Server &other);

	~Server();
private:
	struct pollfd 			fd_list[1024];
	int						_port;
	std::string				_password;
	std::vector<User *>		_users;
	Logger					logger;
	std::vector<Channel *>	_channels; //https://datatracker.ietf.org/doc/html/rfc1459#section-1.3

	void						StartLogMessage();
	void						handleRequest(char *request, User &user);
	std::vector<Command>		parseRequest(std::string const &request, User &);
	bool						checkIfNickRegistered(const std::string &nick);

	//request handling implementations
	void						executeCommand(Command const &);
	void						handleUser(Command const &);
	void						handlePassword(Command const &);
	void						handleSetNick(Command const &);
	void						handlePrivateMessage(Command const &);
	void						handlePing(Command const &);
	void 						handlePong(const Command &command);
	void						handleQuit(const Command &);
	void 						handleNoticeMessage(Command const &);
	void 						handleWho(const Command &command);
	void						createAndSendMessageOfTHeDay(const User &user);
	void						sendMOTD(const User &user);
	void 						sendError(Command const &command , int errorCode );
	void 						killUser(User const &);
	void 						printAllUsers(const Command &command);
	//request handling implementations

	void						messageOfTHeDay(User &user);
	Server();



};

#endif //MY_IRC_SERVER_HPP
