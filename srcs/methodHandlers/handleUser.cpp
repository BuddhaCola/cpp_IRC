#include "../../includes/Server.hpp"

//https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.3
//Command: USER
//Parameters: <user> <mode> <unused> <realname>
// Numeric Replies:
//           ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED
void Server::handleUser(const Command &command) {
	User	&user = command.getUser();

	if (command.getArguments().size() < 4) {
		return sendError(command, ERR_NEEDMOREPARAMS);
	}
	user.setUsername(command.getArgument(0));
	user.setRealname(command.getArgument(3));
}