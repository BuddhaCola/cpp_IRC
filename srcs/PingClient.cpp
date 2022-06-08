//
// Created by Whistler Archangel on 6/4/22.
//
#include "../includes/Server.hpp"


void Server::pingClient()
{
	std::time_t timeNow;
	std::string pingString;
	for (int i = 0; i < _users.size(); i++) {
		User &user  = *_users.at(i);
		pingString = "PING " + std::to_string(user.getTimestamp()) + "\r\n";
		logger.logUserMessage(pingString, user, OUT);
		write(user.getFd(), pingString.c_str(), pingString.length());
		timeNow = std::time(NULL);
		if (timeNow - user.getTimestamp() >= TIMEOUT) {
			killUser(user);
		}
	}
}