#include "../../includes/Server.hpp"

//Numeric Replies:
//ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
//ERR_NOTONCHANNEL
void Server::handlePart(const Command &command) {
	if (command.getArguments().size() < 1) {
		sendError(command, ERR_NEEDMOREPARAMS);
		return;
	}
	Channel	*channel = findChannel(command.getArgument(0));
	User	&user = command.getUser();
	if (!channel) {
		sendError(command, ERR_NOTONCHANNEL);
		return;
	}
	if (std::find(user.getChannels().begin(), user.getChannels().end(), channel) == user.getChannels().end())
	{
		sendError(command, ERR_NOSUCHCHANNEL);
		return;
	}
	removeUserFromChannel(user, *channel, user.getUserInfoString() + " PART " + channel->getName());
}