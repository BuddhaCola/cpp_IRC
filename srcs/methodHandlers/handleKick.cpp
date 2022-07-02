#include "../../includes/Server.hpp"

std::vector<User *> Server::collectToKickList(const Command &command, Channel *channel) {
	std::vector<User *>			toKick;
	std::vector<std::string>	usersToKick;
	std::string					inputArgumentCopy = command.getArgument(1);
	size_t 						current = 0;

	while (true) {
		current = inputArgumentCopy.find(',');
		if (current == std::string::npos)
			break;
		usersToKick.push_back(inputArgumentCopy.substr(0, current));
		inputArgumentCopy = inputArgumentCopy.substr(current + 1);
	}
	for (int i = 0; i < usersToKick.size(); ++i) {
		std::string	currentUserNick = usersToKick.at(i);
		User		*currentUser;
		currentUserNick = currentUserNick;
		currentUser  = channel->findUserByNick(currentUserNick);
		if (!currentUser) {
			sendError(command, ERR_NOSUCHNICK); //TODO fuuuuuuck
			continue;
		}
		toKick.push_back(currentUser);
	}
	return toKick;
}
//KICK #Finnish John :Speaking English
//Numeric Replies:
//ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
//ERR_BADCHANMASK                 ERR_CHANOPRIVSNEEDED
//ERR_NOTONCHANNEL
void Server::handleKick(const Command &command) {
	if (command.getArguments().size() < 2) {
		sendError(command, ERR_NEEDMOREPARAMS);
		return;
	}
	Channel					*channel;
	User					&user = command.getUser();
	std::vector<User *>		toKick;
	std::string	reason;
	channel = findChannel(command.getArgument(0));
	if (!channel) {
		sendError(command, ERR_NOSUCHCHANNEL);
		return;
	}
	if (!channel->isOperator(&user)) {
		sendError(command, ERR_CHANOPRIVSNEEDED);
		return;
	}
	toKick = collectToKickList(command, channel);
	if (command.getArguments().size() > 2) {
		reason = command.getArgument(2);
	}
	else {
		reason = "kicked by " + user.getNick();
	}
	for (int i = 0; i < toKick.size(); ++i) {
		std::string	msg = user.getUserInfoString() + " KICK " + channel->getName() + " " + toKick.at(i)->getNick() + " :" + user.getNick();
		removeUserFromChannel(*toKick.at(i), *channel, msg);
	}
}