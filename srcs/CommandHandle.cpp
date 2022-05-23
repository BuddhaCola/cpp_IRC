#include "../includes/Server.hpp"

void	Server::executeCommand(Command const &command){
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
			std::cerr << "PING method is not implemented" << std::endl;
			return;
		case PONG:
			std::cerr << "PONG method is not implemented" << std::endl;
			return;
		case QUIT:
			std::cerr << "QUIT method is not implemented" << std::endl;
			return;
		default:
			break;
	}
	if (!command.getUser().isAuthorized())
	{
		std::cout << "unauthorized request from _user fd |" << command.getUser().getFd() << "| nick |" << command.getUser().getNick() << "|\n" << command.typeToString() << std::endl;
	}
	else {
		//not allowed for unauthorized users
		switch (command.getType()) {
			case PRIVMSG :
				handlePrivateMessage(command);
				break;
			case NOTICE:
				std::cerr << "NOTICE method is not implemented" << std::endl;
				break;
			case JOIN:
				std::cerr << "JOIN method is not implemented" << std::endl;
				break;
			case OPER:
				std::cerr << "OPER method is not implemented" << std::endl;
				break;
			case KILL:
				std::cerr << "KILL method is not implemented" << std::endl;
				break;
			case KICK:
				std::cerr << "KICK method is not implemented" << std::endl;
				break;
			case LIST:
				std::cerr << "LIST method is not implemented" << std::endl;
				break;
			case WHO:
				std::cerr << "WHO method is not implemented" << std::endl;
				break;
			default:
				break;
		}
	}
}

#include <sstream>
void printRequest(char *request, User &user) {
	std::stringstream out;

	out << currentTime() << user << ' ';
	out << "\n\"" << request << '\"' << std::endl;
	std::cout << out.str() << std::endl;
}

void Server::handleRequest(char *request, User &user) {
	std::vector<Command> commands;

	printRequest(request, user);
	commands = parseRequest(std::string(request), user);
	for (std::vector<Command>::iterator it = commands.begin(); it != commands.end(); ++it) {
		executeCommand(*it);
	}
}

std::vector<Command> Server::parseRequest(std::string const &request, User &user) {
	std::vector<Command>	commands;
	std::stringstream		stream(request);
	std::string				current;

	while (std::getline(stream, current)) {
		if (current.at(current.size() - 1) == '\r') {
			current = current.substr(0, current.size() - 1);
		}
		try {
			commands.push_back(Command(current, user));
		}
		catch (FtException &e) {
				std::cerr << "unrecognized request: \"" << current << '\"' << std::endl;
			}
		}
		return commands;
}

//https://datatracker.ietf.org/doc/html/rfc2812#section-5.2
void Server::handlePrivateMessage(const Command &command) {
#ifdef MORE_INFO
	std::cout << CYAN << "handlePrivateMessage method invoked" << RESET << std::endl;
#endif
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
#ifdef MORE_INFO
	std::cout << CYAN << "handlePassword method invoked" << RESET << std::endl;
#endif
	if (command.getArguments().size() != 1) {
		std::cout << "something went wrong" << std::endl; //TODO errorhandle
	}
	if (command.getTextPart().empty()) {
		std::cout << "something went wrong" << std::endl; //TODO errorhandle
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
#ifdef MORE_INFO
	std::cout << CYAN << "handleSetNick method invoked" << RESET << std::endl;
#endif
	if (command.getArguments().size() != 1)
		std::cout << "something went wrong" << std::endl; //TODO errorhandle
	User &user = command.getUser();
	std::string	nickToSet = trim(command.getArgument(0));
	nickToSet = toLowercase(nickToSet);
	if (!validateString(nickToSet))
		return;
	user.setNick(nickToSet);
#ifdef MORE_INFO
	std::cout << CYAN << "_user fd " << user.getFd() << " has nick set to |" << user.getNick() << "|" << RESET << std::endl;
#endif
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