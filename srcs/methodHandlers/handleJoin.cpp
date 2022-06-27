#include "../../includes/Server.hpp"

void Server::handleJoin(const Command &command) {
	//TODO errorcheck
	std::string const	&channelName = command.getArgument(0);
	User				&user = command.getUser();
	Channel				*channel;

	if (channelName.at(0) != '#') {
		sendError(command, 403);
		return;
	}
	channel = findChannel(channelName);
	if (!channel) {
		try {
			//channel does not exist
			channel = new Channel(channelName);
			_channels.push_back(channel);
			user.addChannel(channel);
			channel->addUser(user);
			channel->setOper(&user);
		}
		catch (FtException &e) {
			sendError(command, ERR_NOSUCHCHANNEL);
			return;
		}
	}
	else {
		if (std::find(user.getChannels().begin(), user.getChannels().end(), channel) == user.getChannels().end()) {
			user.addChannel(channel);
			channel->addUser(user);
		}
	}
	sendMessageToChannel(*channel, ":" + user.getUserInfoString() + " " + "JOIN" + " :" + channel->getName() + "\r\n"); //TODO bullshit

	std::stringstream names;
	std::vector<User *> users = channel->getUsers();
	std::vector<User *> operators = channel->getOperators();
	names << ":My_IRC 353 " << user.getNick() << " = " << channel->getName() <<
	" :"; //TODO bullshit

	for (std::vector<User *>::iterator it = users.begin(); it != users.end(); ++it) {
		if (channel->isOperator((*it))) {
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