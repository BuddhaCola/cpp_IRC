#include "../../includes/Server.hpp"

//https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.4
//Numeric Replies:
//
//ERR_NEEDMOREPARAMS              RPL_YOUREOPER
//ERR_NOOPERHOST                  ERR_PASSWDMISMATCH
void Server::handleOper(const Command &command) {
	if (command.getArguments().size() < 2) {
		return sendError(command, ERR_NEEDMOREPARAMS);
	}
	User	&user = command.getUser();
	if (command.getArgument(0).empty() || toLowercase(command.getArgument(0)) != user.getNickLowercase()) {
		return sendError(command, ERR_NEEDMOREPARAMS);
	}
	if (command.getArgument(1) != OPERPASS) {
		return sendError(command, ERR_PASSWDMISMATCH);
	}
	user.setOper(true);
	return sendReply(command, RPL_YOUREOPER);
}