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

typedef struct Response {
	int			code;
	std::string	body;
}	Response;

class Server {
public:
	Server(int, std::string);
	int 						creat_listen_socket(int);
	void						startLoop(int);
	std::string					getPassword() const;
	int 						getPort() const;
	Server & operator= (const Server &other);

	~Server();
private:
	int						_port;
	std::string				_password;
	std::vector<User *>		_users;
	std::vector<Channel *>	_channels; //https://datatracker.ietf.org/doc/html/rfc1459#section-1.3
	Logger					logger;
	
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
	void						handleQuit(const Command &);
	void						createAndSendMessageOfTHeDay(const User &user);
	void						sendMOTD(const User &user);
	Server();
};

#endif //MY_IRC_SERVER_HPP
