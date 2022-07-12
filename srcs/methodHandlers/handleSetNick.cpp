#include "../../includes/Server.hpp"

//https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.2
void Server::handleSetNick(const Command &command) {
	std::stringstream logStream;
	if (command.getArguments().size() < 1) {
		sendError(command, ERR_NONICKNAMEGIVEN);
		return;
	}
	User &user = command.getUser();
	std::string	nickToSet = trim(command.getArgument(0));
	if (checkIfNickRegistered(nickToSet)) {
		sendError(command, ERR_NICKNAMEINUSE);
		return;
	}
	if (nickToSet.size() < 9 && validateString(nickToSet)) {
		user.setNick(nickToSet);
		user.setNickLowercase(toLowercase(nickToSet)); //move it
	}
	else {
		sendError(command, ERR_ERRONEUSNICKNAME);
		return;
	}
	logStream << "set nick user " << user;
	logger.logMessage(logStream, INFO);
}