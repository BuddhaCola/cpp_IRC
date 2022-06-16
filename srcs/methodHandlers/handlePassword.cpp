#include "../../includes/Server.hpp"

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
			registerUserAndSendMOTD(user);
			return;
		}
		else {
			//do something //TODO
		}
	}
}