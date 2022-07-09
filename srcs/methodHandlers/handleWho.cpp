//
// Created by Whistler Archangel on 6/10/22.
//
#include "../../includes/Server.hpp"

void Server::printChannel(const Command &command)
{
	User &user = 						command.getUser();
	Channel *channel = 					findChannel(command.getArgument(0));

	std::string msg;
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
			if (channel->isOperator((*it)) || (*it)->isOper()) {
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
	std::string msg1 = ":My_IRC " + std::to_string(RPL_WHOREPLY) + " " + user
			.getNick() + " * ";

	for (std::vector<User*>::iterator it = _users.begin(); it != _users.end(); ++it) {
		if ((*it)->getNick() == command.getArgument(0)) {
			msg1 += (*it)->getUsername() + " " + (*it)->getNick() +
					+ " " + (*it)->getIp() + " My_IRC " + (*it)->getNick() + " H :0 "
					+ (*it)->getRealname() + "\r\n";
			write(user.getFd(), msg1.c_str(), msg1.length());
			logger.logUserMessage(msg1, user, OUT);
		} else if (command.getArgument(0).empty()){
			std::string msg2 = ":My_IRC " + std::to_string(RPL_WHOREPLY) + " " + user
					.getNick() + " * ";
			msg2 += (*it)->getUsername() + " " + (*it)->getNick() +
					+ " " + (*it)->getIp() + " My_IRC " + (*it)->getNick() + " H :0 "
					+ (*it)->getRealname() + "\r\n";
		}
	}
	sendReply(command, RPL_ENDOFWHO);
}

void Server::handleWho(const Command &command)
{
	User &user = 						command.getUser();
	std::vector<Channel *> &channels = 	user.getChannels();

	if (command.getArguments().size() == 0) {
		return sendError(command, ERR_NEEDMOREPARAMS);
	}
	if (command.getArgument(0).at(0) == '#') {
		printChannel(command);
	}
	else {
		printUser(command);
	}
}
