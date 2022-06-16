//
// Created by Whistler Archangel on 6/10/22.
//
#include "../../includes/Server.hpp"

void Server::printChannel(const Command &command)
{
	User &user = 						command.getUser();
	std::vector<Channel *> &channels = 	user.getChannels();
	Channel *channel = 					findChannel(command.getArgument(0));

	std::string msg;


//	who #kj
//	:irc.ircnet.su 352 iu #kj 9 178.205.9.166 irc.ircnet.su iu H :0 =
//	:irc.ircnet.su 352 iu #kj 5 178.205.9.166 irc.ircnet.su op H :0 8
//	:irc.ircnet.su 352 iu #kj 1 178.205.9.166 irc.ircnet.su _Ra- H@ :0 4
//	:irc.ircnet.su 315 iu #kj :End of /WHO list.
	if (channel)
	{
		std::string channelName = channel->getName();
		std::vector<User * >::reverse_iterator it;
		std::vector<User *>	usersOfChannel = channel->getUsers();
		for (it = usersOfChannel.rbegin(); it != usersOfChannel.rend(); ++it) {
			msg = ":My_IRC " + std::to_string(RPL_WHOREPLY) + " " + user
			.getNick();
			msg += " " + channelName + " " + (*it)->getUsername() + " "
			+ (*it)->getIp() + " My_IRC " + (*it)->getNick() + " H";
			if (channel->isOperator((*it))) {
				msg += '@';
			}
			 msg += " :0 " + (*it)->getRealname() + "\r\n";

			write(user.getFd(), msg.c_str(), msg.length());
			logger.logUserMessage(msg, user, OUT);
		}
	}
	sendReply(command, RPL_ENDOFWHO);
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
		sendError(command, ERR_NEEDMOREPARAMS);
		if (!channels.empty()) {
			return;
		}
		else if (!this->_users.empty()) {
			printUsers(command);
			return;
		}
	}
	else if (command.getArgument(0).at(0) == '#') {
			printChannel(command);
	}
	else {
		printUser(command);
	}
}
