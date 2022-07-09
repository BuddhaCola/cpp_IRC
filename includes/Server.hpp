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
#include "My_irc.hpp"
#include "Logger.hpp"
#include "allAnswers.hpp"

#define MAX_USERS 		1024
#define POLL_TIMEOUT 	40000
#define TIMEOUT 		180

class Server {
public:
	Server(int, std::string);
	void 						createFdList(int);
	User * 						checkFdUser(int i);
	void 						pollDefault(int listen_sock);
	int 						createListenSocket(int);
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
	std::string					_serverName;
	std::vector<User *>			_users;
	std::vector<Channel *>		_channels; //https://datatracker.ietf.org/doc/html/rfc1459#section-1.3
	Logger						logger;
	std::string					_botName;

	void						StartLogMessage();
	void						handleRequest(char *request, User &user);
	std::vector<Command>		parseRequest(std::string const &request, User &);
	void						executeCommand(Command const &);
	bool						checkIfNickRegistered(const std::string &nick);
	Channel						* findChannel(const std::string &channel);
	void						registerUserAndSendMOTD(User &user);
	void						createAndSendMessageOfTHeDay(const User &user);
	void						removeUserFromAllChannels(User &user, const std::string &reason);
	void						sendMessageToChannel(const Channel &channel, std::string string);
	void						sendMessageToChannel(Command const &);
	void						sendMessageToUser(const Command &, std::string reason);
	void 						sendError(Command const &, int);
	void 						sendReply(Command const &, int);
	void						killUser(User &, std::string );
	void						checkIfChannelEmpty(Channel *);
	void						removeUserFromChannel(User &, Channel &, const std::string &);
	void						printUser(const Command &);
	void						printChannel(const Command &);
	User						*findUserByNick(const std::string &);
	std::vector<User *>			collectToKickList(const Command &, Channel *);
	void						handleBotMessage(const Command &);

	void						handleUser(Command const &);
	void						handlePassword(Command const &);
	void						handleSetNick(Command const &);
	void						handlePrivateMessage(const Command &);
	void						handlePing(const Command  &);
	void						handleOper(const Command  &);
	void 						handlePong(const Command &);
	void						handleQuit(const Command &);
	void 						handleNoticeMessage(const Command &);
	void						handleJoin(const Command &command);
	void						handleKick(const Command &);
	void						handleKill(const Command &);
	void 						handleWho(const Command &command);
	void						handlePart(const Command &command);

	void						botGreeting(User &user);
	void						botListUsers(User &user);
	void						botListChannels(User &user);
};
#endif //MY_IRC_SERVER_HPP