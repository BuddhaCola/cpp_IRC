//
// Created by Whistler Archangel on 6/4/22.
//
#include "../includes/Server.hpp"

void Server::pingClient()
{
	std::time_t timeNow;
	std::string pingString("Ping 1648063017\r\n");
	for (int i = 1; i < MAX_USERS; i++) {
		if (arr_timestamp[i] != -1) {
			write(fd_list[i].fd, pingString.c_str(), pingString.length());
			timeNow = std::time(NULL);
			if (timeNow - arr_timestamp[i] >= TIMEOUT) {
				for (std::vector<User*>::iterator it = _users.begin(); it !=
				_users.end(); it++) {
					if((*it)->getFd() == fd_list[i].fd) {
						_users.erase(it);
						break;
					}
				}
				if (fd_list[i].fd != -1) {
					close(fd_list[i].fd);
				}
				std::cout << "close_fd[" << fd_list[i].fd << "]\n";
				fd_list[i].fd = -1;
				arr_timestamp[i] = -1;
			}
		}
	}
}