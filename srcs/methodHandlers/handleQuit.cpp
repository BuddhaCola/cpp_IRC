#include "../../includes/Server.hpp"

void Server::handleQuit(const Command &command) {
	User &user = command.getUser();
	killUser(user);
}