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
		std::cout << "unauthorized request from user fd |" << command.getUser().getFd() << "| nick |" << command.getUser().getNick() << "|\n" << CommandNames[command.getType()] << std::endl;
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
				std::cout << "undefined command" << command.getBody() << std::endl;
		}
	}
}

#include <sstream>
void printRequest(char *request, User &user) {
	std::stringstream out;

	out << "|USER ";
	if (!user.getNick().empty())
		out << user.getNick() << ' ';
	out << "fd " << user.getFd();
	out << "\n\"" << request << '\"' << std::endl;
	std::cout << out.str() << std::endl;
}

void Server::handleRequest(char *request, User &user) {
	std::vector<Command> commands;

	printRequest(request, user);
	commands = parseRequest(std::string(request), user);
	if (commands.empty()) {
#ifdef MORE_INFO
		std::cout << CYAN "|unrecognized request: " << request << RESET << std::endl;
#endif
	}
	for (std::vector<Command>::iterator it = commands.begin(); it != commands.end(); ++it) {
		executeCommand(*it);
	}
}

std::vector<Command> Server::parseRequest(std::string const &request, User &user) {
	std::vector<Command>	commands;
	std::string				strRequest;

	strRequest = std::string(request);
	size_t pos = 0;
	std::string commandBody;
	while ((pos = strRequest.find("\r\n")) != std::string::npos) {
		commandBody = strRequest.substr(0, pos);
//		std::cout << commandBody << std::endl;
		commands.push_back(Command(commandBody, user));
		strRequest = strRequest.erase(0, pos + 2);
	}
	return commands;
}

void Server::handlePrivateMessage(const Command &command) {
#ifdef MORE_INFO
	std::cout << CYAN << "handlePrivateMessage method invoked" << RESET << std::endl;
#endif
	std::string body = command.getBody();
	std::string reciverNick;
	User		*reciver;

	reciverNick = "";
	for (std::vector<User*>::iterator it = this->_users.begin(); it != this->_users.end(); ++it) {
		if ((*it)->getNick() == reciverNick) {
			reciver = (*it);
			break;
		}
	}
}

void Server::handlePassword(const Command &command) {
#ifdef MORE_INFO
	std::cout << CYAN << "handlePassword method invoked" << RESET << std::endl;
#endif
	std::string	userInput = trim(command.getBody());
	User	&user = command.getUser();

	if (!this->_password.empty()) {
		if (userInput == this->getPassword()) {
			user.setAuthorized(true);
			return;
		}
		else {
			//do something
		}
	}
	return;
}

void Server::handleSetNick(const Command &command) {
#ifdef MORE_INFO
	std::cout << CYAN << "handleSetNick method invoked" << RESET << std::endl;
#endif
	User user = command.getUser();
	std::string	nickToSet = trim(command.getBody());
	if (!validateString(nickToSet))
		return;
	user.setNick(nickToSet);
#ifdef MORE_INFO
	std::cout << CYAN << "user fd " << user.getFd() << " has nick set to |" << user.getNick() << "|" << RESET << std::endl;
#endif
	return;
}

void Server::handleUser(const Command &command) {
	int							fd = command.getUser().getFd();
	std::vector<std::string>	args = splitString(command.getBody());

	std::stringstream out;
	out << "splitted USER command arguments:\n";
	int i = 0;
	for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); ++it) {
		out << '|' << i++ << "|" << (*it) << '\n';
	}
	std::cout << out.str() << std::endl;
	//"001    RPL_WELCOME\n\"Welcome to the Internet Relay Network\n<nick>!<user>@<host>\""
}