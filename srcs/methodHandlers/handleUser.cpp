#include "../../includes/Server.hpp"

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
		//ERR_NEEDMOREPARAMS
	}
	user.setUsername(command.getArgument(0));
	user.setRealname(command.getArgument(3));
	registerUserAndSendMOTD(user);
}