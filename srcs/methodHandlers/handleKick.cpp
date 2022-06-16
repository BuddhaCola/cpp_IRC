#include "../../includes/Server.hpp"

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
	Channel		*channel;
	User		&user = command.getUser();
	User		*toKick;
	std::string	reason;
	channel = findChannel(command.getArgument(0));
	if (!channel) {
		sendError(command, ERR_NOSUCHCHANNEL);
		return;
	}
	toKick = channel->findUserByNick(command.getArgument(1));
	if (!toKick) {
		sendError(command, ERR_NOTONCHANNEL);
		return;
	}
	if (!channel->isOperator(&user)) {
		sendError(command, ERR_CHANOPRIVSNEEDED);
		return;
	}
	if (command.getArguments().size() > 2) {
		reason = command.getArgument(2);
	}
	else {
		reason = ": kicked by " + user.getNick();
	}
	removeUserFromChannel(*toKick, *channel, reason);
	checkIfChannelEmpty(channel);
}