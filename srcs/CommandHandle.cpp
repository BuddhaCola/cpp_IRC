#include <sstream>
#include "../includes/Server.hpp"

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
			logMessage(logStream, DEV);
			return;
		case QUIT:
			logStream << "QUIT method is not implemented" << std::endl;
			logMessage(logStream, DEV);
			return;
		default:
			break;
	}
	if (!command.getUser().isAuthorized())
	{
		logStream << "unauthorized request from " << command.getUser();
		logMessage(logStream, ERROR);
	}
	else {
		//not allowed for unauthorized users
		switch (command.getType()) {
			case PRIVMSG :
				handlePrivateMessage(command);
				break;
			case NOTICE:
				logStream << "NOTICE method is not implemented" << std::endl;
				logMessage(logStream, DEV);
				break;
			case JOIN:
				logStream << "JOIN method is not implemented" << std::endl;
				logMessage(logStream, DEV);
				break;
			case OPER:
				logStream << "OPER method is not implemented" << std::endl;
				logMessage(logStream, DEV);
				break;
			case KILL:
				logStream << "KILL method is not implemented" << std::endl;
				logMessage(logStream, DEV);
				break;
			case KICK:
				logStream << "KICK method is not implemented" << std::endl;
				logMessage(logStream, DEV);
				break;
			case LIST:
				logStream << "LIST method is not implemented" << std::endl;
				logMessage(logStream, DEV);
				break;
			case WHO:
				logStream << "WHO method is not implemented" << std::endl;
				logMessage(logStream, DEV);
				break;
			default:
				break;
		}
	}
}

void Server::handleRequest(char *request, User &user) {
	std::vector<Command> commands;

	logUserMessage(std::string(request), user, IN);
	commands = parseRequest(std::string(request), user);
	for (std::vector<Command>::iterator it = commands.begin(); it != commands.end(); ++it) {
		executeCommand(*it);
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
				logMessage(logStream, ERROR);
			}
		}
		return commands;
}

//https://datatracker.ietf.org/doc/html/rfc2812#section-5.2
void Server::handlePrivateMessage(const Command &command) {
	std::string message = command.getTextPart();
	std::string reciverNick(command.getArgument(0)); //???;
	User		*reciver = 0;

	for (std::vector<User*>::iterator it = this->_users.begin(); it != this->_users.end(); ++it) {
		if ((*it)->getNick() == reciverNick) {
			reciver = (*it);
			break;
		}
	}
	if (reciver)
		write(reciver->getFd(), message.c_str(), message.size());
	else
	{
		//     401    ERR_NOSUCHNICK
		//              "<nickname> :No such nick/channel"
	}
}

//https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.1
void Server::handlePassword(const Command &command) {
	std::stringstream logStream;

	if (command.getArguments().size() != 1) {
			logStream << "something went wrong" << std::endl; //TODO errorhandle
			logMessage(logStream, ERROR);
	}
	if (command.getTextPart().empty()) {
			logStream << "something went wrong" << std::endl; //TODO errorhandle
			logMessage(logStream, ERROR);
	}
	std::string	userInput = trim(command.getTextPart());
	User	&user = command.getUser();

	if (!this->_password.empty()) {
		if (userInput == this->getPassword()) {
			user.setAuthorized(true);
			return;
		}
		else {
			//do something //TODO
		}
	}
}

//https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.2
void Server::handleSetNick(const Command &command) {
	std::stringstream logStream;

	if (command.getArguments().size() != 1) {
		logStream << "something went wrong" << std::endl; //TODO errorhandle
		logMessage(logStream, ERROR);
	}
	User &user = command.getUser();
	std::string	nickToSet = trim(command.getArgument(0));
	nickToSet = toLowercase(nickToSet);
	if (validateString(nickToSet))
		user.setNick(nickToSet);
	else {
		//errorhandle
		logStream << "FAILED ";
	}
	logStream << "set nick user " << user;
	logMessage(logStream, INFO);
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

	}
	user.setUsername(command.getArgument(0));
	user.setRealname(command.getArgument(3));

	//затычка
	std::string mes_376 = ":IRC 376 \r\n";
	write(fd, mes_376.c_str(), mes_376.length());
}

void Server::handlePing(const Command &command) {
	std::string reply;
	User &user = command.getUser();
	reply = "PONG ";
	if (!command.getArguments().empty())
		reply = reply.append(command.getArgument(0));
	write(user.getFd(), reply.c_str(), reply.size());
	logUserMessage(reply, command.getUser(), OUT);
}
