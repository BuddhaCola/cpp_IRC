#ifndef MY_IRC_SERVER_HPP
#define MY_IRC_SERVER_HPP

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <iostream>

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

#define MAX_USERS 1024

class Server {
public:
	Server(int, std::string);
	void 						createFdList(int);
	void pollDefault(int listen_sock);
	void 						sendErrorToUser(Command const &);
	void 						ServerMessageToUser(Command const &command);
	int 						creat_listen_socket(int);
	void						startLoop(int);
	std::string					getPassword() const;
	int 						getPort() const;
	Server & operator= (const Server &other);

	~Server();
private:
	struct pollfd 			fd_list[1024];
	int						_port;
	std::string				_password;
	std::vector<User *>		_users;
	std::vector<Channel *>	_channels;
	int						_flagReg;
	Logger					logger;

	void						StartLogMessage();
	void						handleRequest(char *request, User &user);
	std::vector<Command>		parseRequest(std::string const &request, User &);
	bool						checkIfNickRegistered(const std::string &nick);

	//request handling implementations
	int 						getFlagReg() const;
	void						executeCommand(Command const &);
	void						handleUser(Command const &);
	void						handlePassword(Command const &);
	void						handleSetNick(Command const &);
	void						handlePrivateMessage(Command const &);
	void						handlePing(Command const &);
	void						handleQuit(const Command &);
	void						messageOfTHeDay(User &user);
	Server();



};

#endif //MY_IRC_SERVER_HPP
