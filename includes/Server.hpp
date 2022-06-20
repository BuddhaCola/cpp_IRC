#ifndef MY_IRC_SERVER_HPP
#define MY_IRC_SERVER_HPP

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sys/select.h>
#include <poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>

#include "User.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include "my_irc.hpp"
#include "Logger.hpp"
#include "allAnswers.hpp"

#define MAX_USERS 		1024
#define POLL_TIMEOUT 	30000
#define TIMEOUT 		80

class Server {
public:
	Server(int, std::string);
	void 						createFdList(int);
	User * 						checkFdUser(int i);
	void 						readFromBuffer(int i);
	void 						pollDefault(int listen_sock);
	int 						creatListenSocket(int);
	void						mainLoop(int);
	std::string					getPassword() const;
	int 						getPort() const;
	void 						pingClient();
	Server 						&operator=(const Server &other);
	Server();
	~Server();
private:
	struct pollfd 				fd_list[1024];
	int							_port;
	std::string					_password;
	std::vector<User *>			_users;
	std::vector<Channel *>		_channels; //https://datatracker.ietf.org/doc/html/rfc1459#section-1.3
	Logger						logger;

	void						StartLogMessage();
	void						handleRequest(char *request, User &user);
	std::vector<Command>		parseRequest(std::string const &request, User &);
	void						executeCommand(Command const &);
	bool						checkIfNickRegistered(const std::string &nick);
	Channel * findChannel(const std::string &channel);
	void						registerUserAndSendMOTD(User &user);
	void						createAndSendMessageOfTHeDay(const User &user);
	void						privmsgToUser(const Command &command);
	void						privmsgToChannel(const Command &command);
	void						removeUserFromAllChannels(User &user, const std::string &reason);
	void						sendMessageToChannel(const Channel &channel, std::string string);
	void						sendMessageToChannel(Command const &);
	void sendMessageToUser(const Command &, std::string reason);
	void						sendMOTD(const User &user);
	void 						sendError(Command const &, int);
	void 						sendReply(Command const &, int);
	void						killUser(User &user, std::string reason);
	void						checkIfChannelEmpty(Channel *channel);

	void						handleUser(Command const &);
	void						handlePassword(Command const &);
	void						handleSetNick(Command const &);
	void						handlePrivateMessage(const Command &);
	void						handlePing(const Command  &);
	void 						handlePong(const Command &);
	void						handleQuit(const Command &);
	void 						handleNoticeMessage(const Command &);
	void						handleJoin(const Command &command);
	void						handleKick(const Command &);
	void 						handleWho(const Command &command);

	void removeUserFromChannel(User &user, Channel &channel, const std::string &reason);


	void printUser(const Command &command);

	void printChannel(const Command &command);
};
#endif //MY_IRC_SERVER_HPP