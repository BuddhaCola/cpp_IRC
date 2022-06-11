#include <sstream>
#include "../includes/Server.hpp"
#include "../includes/allAnswers.hpp"

void	Server::executeCommand(Command const &command){
	std::stringstream	logStream;
	User				&user = command.getUser();
	//allowed for unauthorized users
	switch (command.getType()) {
		case PASS:
			handlePassword(command);
			registerUserAndSendMOTD(user);
			return;
		case USER:
			handleUser(command);
			registerUserAndSendMOTD(user);
			return;
		case NICK:
			handleSetNick(command);
			registerUserAndSendMOTD(user);
			return;
		default:
			break;
	}
	if (user.getNick().empty() || user.getUsername().empty()) {
		sendError(command, 451);
		return;
	}
	switch (command.getType()) {
		case PING:
			handlePing(command);
			return;
		case PONG:
			handlePong(command);
			return;
		case QUIT:
			handleQuit(command);
			return;
		default:
			break;
	}
	if (!command.getUser().isAuthorized()) {
		//TODO errorhandle "not registered (password not entered)"
	}
	else {
		//not allowed for unauthorized users
		switch (command.getType()) {
			case PRIVMSG :
				handlePrivateMessage(command);
				break;
			case NOTICE:
//				handleNoticeMessage(command);
				break;
			case JOIN:
				handleJoin(command);
				break;
			case OPER:
				logStream << "OPER method is not implemented" << std::endl;
				logger.logMessage(logStream, DEV);
				break;
			case KILL:
				logStream << "KILL method is not implemented" << std::endl;
				logger.logMessage(logStream, DEV);
				break;
			case KICK:
				handleKick(command);
				break;
			case LIST:
				logStream << "LIST method is not implemented" << std::endl;
				logger.logMessage(logStream, DEV);
				break;
			case WHO:
				logStream << "WHO method is not implemented" << std::endl;
				logger.logMessage(logStream, DEV);
				break;
			default:
				break;
		}
	}
}

void Server::handleRequest(char *request, User &user) {
	std::vector<Command> commands;

	user.setTimestamp(std::time(NULL));
	logger.logUserMessage(std::string(request), user, IN);
	commands = parseRequest(std::string(request), user);
	for (std::vector<Command>::iterator it = commands.begin(); it != commands.end(); ++it) {
		try {
			executeCommand(*it);
		}
		catch (std::exception &e) {
			std::stringstream logStream;
			logStream << "FAILED to execute command: " << request;
			logger.logMessage(logStream, ERROR);
		}
	}
}

std::vector<Command> Server::parseRequest(std::string const &request, User &user) {
	std::vector<Command>	commands;
	std::stringstream		stream(request);
	std::stringstream		logStream;
	std::string				current;

	while (std::getline(stream, current)) {
		try {
			if (current.size() <= 1)
				throw FtException();
			if (current.at(current.size() - 1) == '\r') {
				current = current.substr(0, current.size() - 1);
			}
			commands.push_back(Command(current, user));
		}
		catch (FtException &e) {
				logStream << "unrecognized request: \"" << current << '\"' << std::endl;
				logger.logMessage(logStream, ERROR);
			}
		}
		return commands;
}

void Server::registerUserAndSendMOTD(User &user) {
	if (user.isAuthorized() && !user.getNick().empty() && !user.getUsername().empty()) {
		if (!user.isRegistered()) {
			//prevent repeating MOTD sending
			user.setRegistered(true);
			createAndSendMessageOfTHeDay(user);
		}
	}
}

void Server::createAndSendMessageOfTHeDay(const User &user)
{    //затычка
	//TODO move it to the propper method
	std::stringstream stream;
	stream << ":My_IRC 375 " + user.getNick() + " :- irc.ircnet.su Message of the Day -\r\n"; //TODO use
	stream << ":My_IRC 372 " + user.getNick() + " wow\r\n";
	stream << ":My_IRC 376 " + user.getNick() + " :End of /MOTD command.\r\n";
	std::string mes_376 = stream.str();
	write(user.getFd(), mes_376.c_str(), mes_376.length());
}

void Server::handleWho(const Command &command)
{
	if (!command.getArgument(0).empty()) {

		sendReply(command,RPL_ENDOFWHO);
	}
	else
	{
		//TODO printAllChannels();
	}
}

bool Server::checkIfNickRegistered(const std::string &nick) {
	std::string lowerCased = toLowercase(nick);

	for (std::vector<User*>::iterator it = this->_users.begin(); it != this->_users.end(); ++it) {
		if ((*it)->getNickLowercase() == lowerCased) {
			return true;
		}
	}
	return false;
}

Channel *Server::findChannel(const std::string &channel) {
	std::string lowerCased = toLowercase(channel);

	for (std::vector<Channel*>::iterator it = this->_channels.begin(); it != this->_channels.end(); ++it) {
		if ((*it)->getNameLowercased() == lowerCased) {
			return *(it);
		}
	}
	return 0;
}

void Server::killUser(User &user, std::string reason) {
	std::stringstream logStream;

	for (std::vector<User*>::iterator it = this->_users.begin(); it != this->_users.end(); ++it) {
		if ((*it) == &user) {
			removeUserFromAllChannels(user, reason);
			_users.erase(it);
			fd_list[user.getFd() - 3].fd = -1;
			user.~User();
			logStream << "User " << user.getNick() << " was removed from the "
				"server";
			logger.logMessage(logStream, INFO);
			break;
		}
	}
}

void Server::checkIfChannelEmpty(Channel *channel) {
	if (channel->getUsers().empty()) {
		std::vector<Channel *>::iterator it = std::find(_channels.begin(), _channels.end(), channel);
		_channels.erase(it);
	}
}

void Server::removeUserFromChannel(User &user, Channel &channel ,const std::string &reason) {
	sendMessageToChannel(channel, ":" + user.getUserInfoString() + " QUIT :" + reason + "\r\n");
	(channel).removeUser(&user);
	checkIfChannelEmpty(&channel);
}

void Server::removeUserFromAllChannels(User &user, const std::string &reason) {
	std::vector<Channel *> &channels = user.getChannels();
	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it) {
		removeUserFromChannel(user, *(*it), reason);
	}
}

void Server::sendMessageToUser(const Command &command) {
	if (command.getArguments().size() == 0) {
		return sendError(command, ERR_NORECIPIENT);
	}
	if (command.getArguments().size() < 2) {
		return sendError(command, ERR_NOTEXTTOSEND);
	}

	std::stringstream	logStream;
	std::string			reciverNick = command.getArgument(0);
	std::string			message = command.getArgument(1);
	User				*reciver = 0;

	for (std::vector<User*>::iterator it = this->_users.begin(); it != this->_users.end(); ++it) { //TODO move to findUserByNick
		if ((*it)->getNick() == reciverNick) {
			reciver = (*it);
			break;
		}
	}
	if (reciver) {
		std::stringstream qtoSend;
		qtoSend << ':' + command.getUser().getUserInfoString() <<  " " << "PRIVMSG" << " " << reciverNick << " :" << message << "\r\n";
		std::string str = qtoSend.str();
		write(reciver->getFd(), str.c_str(), str.size());
		logger.logUserMessage(str, *reciver, OUT);
	}
	else {
		sendError(command, ERR_NOSUCHNICK);
	}
}

void Server::sendMessageToChannel(Command const &command) {
	if (command.getArguments().size() < 2) {
		return sendError(command, ERR_NEEDMOREPARAMS);;
	}
	Channel *channel = findChannel(command.getArgument(0));
	if (!channel) {
		return sendError(command, ERR_NOSUCHCHANNEL);;
	}
	std::string string = command.getArgument(1);
	std::vector<User *> users = channel->getUsers();
	std::string	tosend;
	//:doom!qr@188.242.23.62 PRIVMSG #wow :hey!
	tosend += ':' + command.getUser().getUserInfoString() + ' ' + command.typeToString() + ' ' + channel->getName() + ' ' + string + "\r\n";
	for (std::vector<User *>::iterator it = users.begin(); it != users.end(); ++it) {
		int fd = (*it)->getFd();
		write(fd, tosend.c_str(), tosend.length());
		logger.logUserMessage(tosend, *(*it), OUT);
	}
}

void Server::sendMessageToChannel(const Channel &channel, std::string string) {
	std::vector<User *>	users = channel.getUsers();

	for (std::vector<User *>::iterator it = users.begin(); it != users.end(); ++it) {
		int fd = (*it)->getFd();
		write(fd, string.c_str(), string.length());
		logger.logUserMessage(string, *(*it), OUT);
	}
}



