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
	if (user.getNick().empty() || user.getUsername().empty()) {
		sendError(command, ERR_NOTREGISTERED);
		return;
	}
	if (!command.getUser().isAuthorized()) {
		return sendError(command, ERR_NOTREGISTERED);
	}
	else {
		//not allowed for unauthorized users
		switch (command.getType()) {
			case PRIVMSG :
				handlePrivateMessage(command);
				break;
			case NOTICE:
				handleNoticeMessage(command);
				break;
			case JOIN:
				handleJoin(command);
				break;
			case OPER:
				handleOper(command);
				break;
			case KILL:
				handleKill(command);
				break;
			case KICK:
				handleKick(command);
				break;
			case LIST:
				logStream << "LIST method is not implemented" << std::endl;
				logger.logMessage(logStream, DEV);
				break;
			case PART:
				handlePart(command);
				break;
			case WHO:
				handleWho(command);
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
		try {
			commands = parseRequest(std::string(request), user);
			for (std::vector<Command>::iterator it = commands.begin(); it != commands.end(); ++it)
				executeCommand(*it);
		}
		catch (std::out_of_range &e) {
			std::stringstream logStream;
			logStream << "FAILED to execute command: " << request;
			logStream << "caught exception of type std::out_of_range: ";
			logStream << e.what();
			logger.logMessage(logStream, ERROR);
		}
		catch (std::exception &e) {
			std::stringstream logStream;
			logStream << "FAILED to execute command: " << request;
			logStream << e.what();
			logger.logMessage(logStream, ERROR);
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

				std::string strError = ":My-IRC 421 " + user.getNick() + " "
					+ current + " :Unknown command\r\n";
				write(user.getFd(), strError.c_str(), strError.length());
				logStream << "unrecognized request: \"" << current << '\"' << std::endl;
				logger.logMessage(logStream, ERROR);
		}
		catch (std::exception &e) {
				logStream << "something went wrong: \"" << current << '\"' << std::endl;
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
			botGreeting(user);
		}
	}
}

void Server::createAndSendMessageOfTHeDay(User &user)
{
	std::stringstream stream;
	stream << "001 * :- Welcome to " + _serverName + " server, " + user
	.getUserInfoString() + "\r\n";
	stream << ":" + _serverName + " 375 " + user.getNick() + " :- " + _serverName +" Message of the Day -\r\n";
	stream << ":" + _serverName + " 372 " + user.getNick() + " Welcome to " + _serverName + " Server!\r\n";
	stream << ":" + _serverName + " 376 " + user.getNick() + " :End of /MOTD command.\r\n";
	std::string mes_376 = stream.str();
	logger.logUserMessage(mes_376, user, OUT);
	send(user.getFd(), mes_376.c_str(), mes_376.length() + 1, 0);
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
			delete &user;
			logStream << user.getUserInfoString() + reason + "\r\n";
			logger.logMessage(logStream, INFO);
			break;
		}
	}
}

void Server::checkIfChannelEmpty(Channel *channel) {
	if (channel->getUsers().empty()) {
		std::vector<Channel *>::iterator it = std::find(_channels.begin(), _channels.end(), channel);
		_channels.erase(it);
		delete (*it);
	}
}

void Server::removeUserFromChannel(User &user, Channel &channel ,const std::string &reason) {
	sendMessageToChannel(channel, ":" + reason + "\r\n");
	(channel).removeUser(&user);
	user.removeChannel(&channel);
	checkIfChannelEmpty(&channel);
}

void Server::removeUserFromAllChannels(User &user, const std::string &reason) {
	std::vector<Channel *> &channels = user.getChannels();
	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it) {
		removeUserFromChannel(user, *(*it), reason);
		if (channels.empty())
			break;
	}
}

void Server::sendMessageToUser(const Command &command, std::string reason)
{
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


	if (toLowercase(reciverNick) == toLowercase(_botName)) {
		handleBotMessage(command);
		return;
	}
	reciver = findUserByNick(reciverNick);
	if (reciver) {
		std::stringstream toSend;
		toSend << ':' + command.getUser().getUserInfoString() << " " <<
		reason << " " << reciverNick << " :" << message << "\r\n";
		std::string str = toSend.str();
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

	tosend += ':' + command.getUser().getUserInfoString() + ' ' + command.typeToString() + ' ' + channel->getName() + ' ' + string + "\r\n";
	for (std::vector<User *>::iterator it = users.begin(); it != users.end(); ++it) {
		int fd = (*it)->getFd();
		if (fd == command.getUser().getFd())
			continue;
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

User *Server::findUserByNick(const std::string &reciverNick) {
	User *reciver = 0;
	std::string	reciverNickLowercased = toLowercase(reciverNick);

	for (std::vector<User*>::iterator it = this->_users.begin(); it != this->_users.end(); ++it) {
		if ((*it)->getNickLowercase() == reciverNickLowercased) {
			reciver = (*it);
			break;
		}
	}
	return reciver;
}

User * Server::findUserByFd(int fd)
{
	User *toFind = 0;
	for (std::vector<User*>::iterator it = this->_users.begin(); it !=
	this->_users.end(); ++it) {
		if ((*it)->getFd() == fd) {
			toFind = (*it);
			break;
		}
	}
	return toFind;
}
