//
// Created by Whistler Archangel on 6/4/22.
//
#include "../includes/Server.hpp"


void Server::pingClient()
{
	std::time_t timeNow;
	std::string pingString("Ping 1648063017\r\n");
	for (int i = 0; i < _users.size(); i++) {
		User &user  = *_users.at(i);
		write(user.getFd(), pingString.c_str(), pingString.length());
		timeNow = std::time(NULL);
		if (timeNow - user.getTimestamp() >= TIMEOUT) {
			fd_list[i].fd = -1;//костыль, без него poll не
			// будет корректно работать так она использует fd_list
			killUser(user);
		}
	}
}