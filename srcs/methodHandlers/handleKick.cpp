#include "../../includes/Server.hpp"

std::vector<User *> Server::collectToKickList(const Command &command, Channel *channel) {
	std::vector<User *>			toKick;
	std::vector<std::string>	usersToKick;
	std::string					inputArgumentCopy = command.getArgument(1);
	size_t 						current = 0;

	while (true) {
		current = inputArgumentCopy.find(',');
		if (current == std::string::npos) {
			if (usersToKick.empty())
				usersToKick.push_back(inputArgumentCopy);
			break;
		}
		usersToKick.push_back(inputArgumentCopy.substr(0, current));
		inputArgumentCopy = inputArgumentCopy.substr(current + 1);
	}
	for (int i = 0; i < usersToKick.size(); ++i) {
		std::string	currentUserNick = usersToKick.at(i);
		User		*currentUser;
		currentUserNick = currentUserNick;
		currentUser  = channel->findUserByNick(currentUserNick);
		if (!currentUser) {
			sendError(command, ERR_NOSUCHNICK);
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
		return sendError(command, ERR_NEEDMOREPARAMS);
	}
	Channel					*channel;
	User					&user = command.getUser();
	std::vector<User *>		toKick;
	std::string	reason;
	channel = findChannel(command.getArgument(0));
	if (!channel) {
		return sendError(command, ERR_NOSUCHCHANNEL);
	}
	if (!channel->isOperator(&user) && !user.isOper()) {
		return sendError(command, ERR_CHANOPRIVSNEEDED);
	}
	toKick = collectToKickList(command, channel);
	for (int i = 0; i < toKick.size(); ++i) {
		std::string	msg = user.getUserInfoString() + " KICK " + channel->getName() + " " + toKick.at(i)->getNick() + " :" + user.getNick();
		removeUserFromChannel(*toKick.at(i), *channel, msg);
	}
}