#include "../../includes/Server.hpp"

//https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.1
void Server::handlePassword(const Command &command) {
	if (this->_password.empty())
		return;
	if (command.getArguments().empty()) {
		sendError(command, ERR_NEEDMOREPARAMS);
		return;
	}
	if (command.getArgument(0).empty()) {
		sendError(command, ERR_NEEDMOREPARAMS);
		return;
	}
	std::string	userInput = trim(command.getArgument(0));
	User	&user = command.getUser();
	if (userInput == this->getPassword()) {
		user.setAuthorized(true);
		registerUserAndSendMOTD(user);
		return;
	}
	else {
		sendError(command, ERR_PASSWDMISMATCH);
		return;
	}
}