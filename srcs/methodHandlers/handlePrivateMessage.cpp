#include "../../includes/Server.hpp"

//https://datatracker.ietf.org/doc/html/rfc2812#section-5.2
void Server::handlePrivateMessage(const Command &command) {
	if (command.getArguments().size() == 0) {
		return sendError(command, ERR_NORECIPIENT);
	}
	if (command.getArguments().size() < 2) {
		return sendError(command, ERR_NOTEXTTOSEND);
	}

	std::string			reciverNick = command.getArgument(0);
	if (reciverNick.at(0) == '#') {
		sendMessageToChannel(command);
	} else {
		sendMessageToUser(command);
	}
}