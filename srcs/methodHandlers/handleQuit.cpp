#include "../../includes/Server.hpp"

void Server::handleQuit(const Command &command) {
	User &user = command.getUser();
	std::string	reason;
	if (!command.getArguments().empty()) {
		reason = command.getArgument(0);
	}
	killUser(user, reason);
}