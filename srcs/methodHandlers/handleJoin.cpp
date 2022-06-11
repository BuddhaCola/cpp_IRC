#include "../../includes/Server.hpp"

void Server::handleJoin(const Command &command) {
	//TODO errorcheck
	std::string const	&channelName = command.getArgument(0);
	User				&user = command.getUser();
	Channel				*channel;

	if (channelName.at(0) != '#') {
		sendError(command, 403);
		return;
//		//TODO bullshit
//		std::string error = ":irc.ircnet.su 403 " + user.getNick() + " " + command.getArgument(0) +" :No such channel";
//		write(user.getFd(), error.c_str(), error.length());
//		logger.logUserMessage(error, user, OUT);
	}
	channel = findChannel(channelName);
	if (!channel) {
		//channel does not exist
		channel = new Channel(channelName);
		_channels.push_back(channel);
		user.addChannel(channel);
		channel->addUser(user);
		channel->setOper(&user);
	}
	else {
		user.addChannel(channel);
		channel->addUser(user);
//		:irc.ircnet.su 353 pop = #ads :pop @realman
//		:irc.ircnet.su 366 pop #ads :End of /NAMES list.
		//list users
	}
	sendMessageToChannel(*channel, ":" + user.getUserInfoString() + " " + "JOIN" + " :" + channel->getName() + "\r\n"); //TODO bullshit

	std::stringstream names;
	std::vector<const User *> users = channel->getUsers();
	names << ":irc.ircnet.su 353 " << user.getNick() << " = " << channel->getName() << " :"; //TODO bullshit

	for (std::vector<User const *>::iterator it = users.begin(); it != users.end(); ++it) {
		if ((*it) == channel->getOper()) {
			names << '@';
		}
		names << (*it)->getNick();
		if ((it + 1) != users.end())
			names << ' ';
	}
	names << "\r\n";
	std::string namesString = names.str();
	write(user.getFd(), namesString.c_str(), namesString.length());
	logger.logUserMessage(namesString, user, OUT);


//	listChannelNames(channel);
//send to everyone :ho!qw@188.242.23.62 JOIN :#j


	//TODO VV bullshit VV
	std::stringstream endOfNamesList;
	endOfNamesList << ":irc.ircnet.su 366 " << user.getNick() << " " << channel->getName() << " :End of /NAMES list.\r\n";
	std::string endOfNamesListString = endOfNamesList.str();
	write(user.getFd(), endOfNamesListString.c_str(), endOfNamesListString.length());
	logger.logUserMessage(endOfNamesListString, user, OUT);
}

void Server::sendMessageToChannel(const Channel &channel, std::string string) {
	std::vector<const User *> users = channel.getUsers();

	for (std::vector<User const *>::iterator it = users.begin(); it != users.end(); ++it) {
		int fd = (*it)->getFd();
		write(fd, string.c_str(), string.length());
		logger.logUserMessage(string, *(*it), OUT);
	}
}