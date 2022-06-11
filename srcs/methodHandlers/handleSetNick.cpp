#include "../../includes/Server.hpp"

//https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.2
void Server::handleSetNick(const Command &command) {
	std::stringstream logStream; //TODO add check if already exist

	if (command.getArguments().size() != 1) {
		logStream << "something went wrong" << std::endl; //TODO errorhandle
		logger.logMessage(logStream, ERROR);
	}
	User &user = command.getUser();
	std::string	nickToSet = trim(command.getArgument(0));
	if (checkIfNickRegistered(nickToSet)) {
		sendError(command, ERR_NICKNAMEINUSE);
		logStream << "nick " << nickToSet << " already registered" <<
				  std::endl; //TODO errorhandle
		logger.logMessage(logStream, ERROR);
		return;
	}
	if (validateString(nickToSet)) {
		user.setNick(nickToSet);
		user.setNickLowercase(toLowercase(nickToSet)); //move it
	}
	else {
		//errorhandle
		logStream << "FAILED ";
		return;
	}
	logStream << "set nick user " << user;
	logger.logMessage(logStream, INFO);
//	registerUserAndSendMOTD(user);
}