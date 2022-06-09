#include "../../includes/Server.hpp"

void Server::handleJoin(const Command &command) {
	//TODO errorcheck
	std::string const	&channelName = command.getArgument(0);
	User				&user = command.getUser();
	Channel				*channel;

	if (channelName.at(0) != '#') {
//		TODO error handle
//		:irc.ircnet.su 403 wew hoh :No such channel
		return;
	}
	channel = findChannel(channelName);
	if (!channel) {
		//channel does not exist
		Channel *newChannel = new Channel(channelName);
		_channels.push_back(newChannel);
		user.addChannel(newChannel);
	}
	else {
		user.addChannel(channel);
		channel->addUser(user);
//		:irc.ircnet.su 353 pop = #ads :pop @realman
//		:irc.ircnet.su 366 pop #ads :End of /NAMES list.
		//list users
	}
}