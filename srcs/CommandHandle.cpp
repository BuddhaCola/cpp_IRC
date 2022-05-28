#include <sstream>
#include "../includes/Server.hpp"
#include "../includes/Command.hpp"

void	Server::executeCommand(Command const &command){
	std::stringstream logStream;
	//allowed for unauthorized users
	switch (command.getType()) {
		case PASS:
			handlePassword(command);
			return;
		case USER:
			handleUser(command);
			return;
		case NICK:
			handleSetNick(command);
			return;
		case PING:
			handlePing(command);
			return;
		case PONG:
			logStream << "PONG method is not implemented" << std::endl;
			logger.logMessage(logStream, DEV);
			return;
		case QUIT:
			handleQuit(command);
			return;
		default:
			break;
	}
	if (!command.getUser().isAuthorized())
	{
		logStream << "unauthorized request from " << command.getUser();
		logger.logMessage(logStream, ERROR);
	}
	else {
		//not allowed for unauthorized users
		switch (command.getType()) {
			case PRIVMSG :
				handlePrivateMessage(command);
				break;
			case NOTICE:
				logStream << "NOTICE method is not implemented" << std::endl;
				logger.logMessage(logStream, DEV);
				break;
			case JOIN:
				logStream << "JOIN method is not implemented" << std::endl;
				logger.logMessage(logStream, DEV);
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
				logStream << "KICK method is not implemented" << std::endl;
				logger.logMessage(logStream, DEV);
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

//https://datatracker.ietf.org/doc/html/rfc2812#section-5.2
void Server::handlePrivateMessage(const Command &command) {
	std::stringstream	logStream;
	if (command.getArguments().size() < 2 || command.getArguments().size() > 2) {
		logStream << "Not enough parameters" << std::endl; //TODO errorhandle
		logger.logMessage(logStream, ERROR);
//		throw FtException();
	}
	std::string			reciverNick = command.getArgument(0); //???;
	std::string			message = command.getArgument(1);
	User		*reciver = 0;

	for (std::vector<User*>::iterator it = this->_users.begin(); it != this->_users.end(); ++it) {
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
	else
	{
		int code;
		code = 401;
		std::stringstream str;
		str << ":My_IRC " << code << " " << command.getUser().getNick() << " " << reciverNick << " :" << "No such nick or channel" << "\r\n"; //:irc.ircnet.su 451 PRIVMSG :You have not registered
		std::string reply = str.str();
		write(command.getUser().getFd(), reply.c_str(), reply.size());
		logger.logUserMessage(reply, command.getUser(), OUT);
		//     401    ERR_NOSUCHNICK
		//              "<nickname> :No such nick/channel"
	}
}

//https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.1
void Server::handlePassword(const Command &command) {
	std::stringstream logStream;

	if (command.getArguments().size() != 1) {
			logStream << "something went wrong" << std::endl; //TODO errorhandle
			logger.logMessage(logStream, ERROR);
			return;
	}
	if (command.getTextPart().empty()) {
			logStream << "something went wrong" << std::endl; //TODO errorhandle
			logger.logMessage(logStream, ERROR);
			return;
	}
	std::string	userInput = trim(command.getTextPart());
	User	&user = command.getUser();

	if (!this->_password.empty()) {
		if (userInput == this->getPassword()) {
			user.setAuthorized(true);
			return;
		}
		else {
			//do something ответ юзеру //TODO
		}
	}
}

//https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.2
void Server::handleSetNick(const Command &command) {
	std::stringstream logStream; //TODO add check if already exist

	if (command.getArguments().size() != 1) {
		logStream << "something went wrong" << std::endl; //TODO errorhandle
		logger.logMessage(logStream, ERROR);
	}
	User &user = command.getUser();
	std::string	nickToSet = trim(command.getArgument(0));
	if (!checkIfNickRegistered(nickToSet)) {
		logStream << "nick" << nickToSet << " already registered" << std::endl; //TODO errorhandle
		logger.logMessage(logStream, ERROR);
	}
	if (validateString(nickToSet)) {
		user.setNick(nickToSet);
		user.setNickLowercase(toLowercase(nickToSet)); //move it
	}
	else {
		//errorhandle
		logStream << "FAILED ";
	}
	logStream << "set nick user " << user;
	logger.logMessage(logStream, INFO);
	messageOfTHeDay(user);
	return;
}

//https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.3
//Command: USER
//Parameters: <user> <mode> <unused> <realname>
// Numeric Replies:
//           ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED
void Server::handleUser(const Command &command) {
	User	&user = command.getUser();
	int		fd = user.getFd();
//	int		mode = stoi(command.getArgument(1)); //https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.5

	if (command.getArguments().size() < 4) {
		//ERR_NEEDMOREPARAMS
	}
	user.setUsername(command.getArgument(0));
	user.setRealname(command.getArgument(3));

}

void Server::messageOfTHeDay(User &user)
{    //затычка
	std::stringstream stream;
	//:IRC 375 pop :- IRC Message of the day -
	//:IRC 372 pop :- IRC Welcome to server!!!
	//:IRC 376 pop :End of /MOTD command
	stream << ":My_IRC 375 " + user.getNick() + " :- irc.ircnet.su Message of the Day -\r\n";
	stream << ":My_IRC 372 " + user.getNick() + " wow\r\n";
	stream << ":My_IRC 376 " + user.getNick() + " :End of /MOTD command.\r\n";
	std::string mes_376 = stream.str();
	write(user.getFd(), mes_376.c_str(), mes_376.length());
}

void Server::handlePing(const Command &command) {
	std::string reply;
	User &user = command.getUser();
	reply = "PONG ";
	if (!command.getArguments().empty())
		reply = reply.append(command.getArgument(0));
	write(user.getFd(), reply.c_str(), reply.size());
	logger.logUserMessage(reply, command.getUser(), OUT);
}

bool Server::checkIfNickRegistered(const std::string &nick) {
	std::string lowerCased = toLowercase(nick);

	for (std::vector<User*>::iterator it = this->_users.begin(); it != this->_users.end(); ++it) {
		if ((*it)->getNickLowercase() == lowerCased) {
			return false;
		}
	}
	return true;
}

void Server::handleQuit(const Command &command) {
	User &user = command.getUser();
	std::stringstream logStream;

	for (std::vector<User*>::iterator it = this->_users.begin(); it != this->_users.end(); ++it) {
		if ((*it) == &user) {
			_users.erase(it);
			logStream << "user " << user.getNick() << " removed from the server";
			logger.logMessage(logStream, INFO);
			break;
		}
	}
}
