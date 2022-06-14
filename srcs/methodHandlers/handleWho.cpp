//
// Created by Whistler Archangel on 6/10/22.
//
#include "../../includes/Server.hpp"

void Server::printChannels(const Command &command)
{
	User &user = 						command.getUser();
	std::string 						channelName;
	std::vector<Channel *> &channels = 	user.getChannels();
	std::string msg = ":My_IRC " + std::to_string(RPL_WHOREPLY) + " " + user
			.getNick() + " * ";

	for (std::vector<Channel *>::iterator it = _channels.begin(); it !=
																  _channels.end(); ++it) {
		msg += channelName;
	}
}

void Server::printUser(const Command &command)
{
	User &user = 						command.getUser();
	std::string msg = ":My_IRC " + std::to_string(RPL_WHOREPLY) + " " + user
			.getNick() + " * ";


	for (std::vector<User*>::iterator it = _users.begin(); it != _users.end(); ++it) {
		if ((*it)->getNick() == command.getArgument(0)) {
			msg += (*it)->getUsername() + " " + (*it)->getNick() +
				   + " " + (*it)->getIp() +" My_IRC " + (*it)->getNick() + " H :0 "
				   + (*it)->getRealname() + "\r\n";
			write(user.getFd(), msg.c_str(), msg.length());
			logger.logUserMessage(msg, user, OUT);
		}
	}
	sendReply(command, RPL_ENDOFWHO);
}

void Server::printUsers(Command const &command)
{
	while (_users.begin() != _users.end()) {
		printUser(command);
	}
	sendReply(command, RPL_ENDOFWHO);

//	for (std::vector<User *>::iterator it = _users.begin();
//		 it != _users.end(); ++it)
//	{
//		if ((*it)->getNick() == command.getArgument(0))
//		{
//			msg += user.getUsername() + " " + user.getNick() +
//				   +" " + user.getIp() + " My_IRC " + user.getNick() + " H :0 "
//				   + user.getRealname() + "\r\n";
//			write(user.getFd(), msg.c_str(), msg.length());
//			logger.logUserMessage(msg, user, OUT);
//		}
//	}
}

void Server::handleWho(const Command &command)
{
	User &user = 						command.getUser();
	std::vector<Channel *> &channels = 	user.getChannels();

	if (command.getArguments().size() == 0) {
		if (!channels.empty()) {
			printChannels(command);
			return;
		}
		else if (!this->_users.empty()) {
			printUsers(command);
			return;
		}
		sendError(command, ERR_NEEDMOREPARAMS);
	}
	else {
		printUser(command);
	}
}
