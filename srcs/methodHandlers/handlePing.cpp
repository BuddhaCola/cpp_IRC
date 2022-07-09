#include "../../includes/Server.hpp"

void Server::handlePing(const Command &command) {
	std::string reply;
	User &user = command.getUser();
	reply = "PONG";
	if (!command.getArguments().empty())
		reply = reply.append(command.getArgument(0)) + "\r\n";
	write(user.getFd(), reply.c_str(), reply.size());
	logger.logUserMessage(reply, command.getUser(), OUT);
}