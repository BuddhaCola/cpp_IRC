#include "../../includes/Server.hpp"

//Numeric Replies:
//ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
//ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
//ERR_CHANNELISFULL               ERR_BADCHANMASK
//ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
//RPL_TOPIC
void Server::handleJoin(const Command &command) {
	if (command.getArguments().size() < 1) {
		return sendError(command, ERR_NEEDMOREPARAMS);
	}
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
	sendMessageToChannel(*channel, ":" + user.getUserInfoString() + " " + "JOIN" + " :" + channel->getName() + "\r\n");

	std::stringstream names;
	std::vector<User *> users = channel->getUsers();
	std::vector<User *> operators = channel->getOperators();
	names << ":" + _serverName +" 353 " << user.getNick() << " = " << channel->getName() << " :";

	for (std::vector<User *>::iterator it = users.begin(); it != users.end(); ++it) {
		if (channel->isOperator((*it)) || (*it)->isOper()) {
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

	std::stringstream endOfNamesList;
	endOfNamesList << ":" + _serverName +" 366 " << user.getNick() << " " << channel->getName() << " :End of /NAMES list.\r\n";
	std::string endOfNamesListString = endOfNamesList.str();
	write(user.getFd(), endOfNamesListString.c_str(), endOfNamesListString.length());
	logger.logUserMessage(endOfNamesListString, user, OUT);
}