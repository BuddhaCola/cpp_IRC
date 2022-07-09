#include "../../includes/Server.hpp"

//Numeric Replies:
//ERR_NOPRIVILEGES                ERR_NEEDMOREPARAMS
//ERR_NOSUCHNICK                  ERR_CANTKILLSERVER
void Server::handleKill(const Command &command) {
	User					&user = command.getUser();
	if (!user.isOper()) {
		return sendError(command, ERR_NOPRIVILEGES);
	}
	if (command.getArguments().empty()) {
		return sendError(command, ERR_NEEDMOREPARAMS);
	}
	User *toKill = findUserByNick(command.getArgument(0));
	if (!toKill){
		if (findChannel(command.getArgument(0))) {
			return sendError(command, ERR_CANTKILLSERVER);
		}
		return sendError(command, ERR_NOSUCHNICK);
	}
	std::string reason = toKill->getUserInfoString() + " QUIT ";
		reason += " :killed by " + user.getNick();
	if (command.getArguments().size() > 1) {
		reason += " with reason: " + command.getArgument(1);
	}
	killUser(*toKill, reason);
}