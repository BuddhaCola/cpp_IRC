#include <iostream>
#include "../includes/My_irc.hpp"
#include "../includes/Server.hpp"
#include <stdlib.h>

void validateArguments(int ac, char **av) {
	if (ac < 2) {
		std::cout << "Usage: ./ircserv <port> <Password>" << std::endl;
		std::cout << "too low _arguments!" << std::endl;
		exit(-1);
	}
	if (ac > 2) {
		if (ac > 3) {
			std::cout << "Usage: ./ircserv <port> <Password>" << std::endl;
			std::cerr << "Too many _arguments!" << std::endl;
			exit(-1);
		}
		if (strlen(av[2]) == 0) {
			std::cerr << "Password can't be an empty string" << std::endl;
			exit(-1);
		}
	}
	try {
		if ((std::stoi(av[1])) < 0)
			throw FtException();
	}
	catch (std::exception &e) {
		std::cerr << "invalid argument: port" << std::endl;
#ifdef MORE_INFO
		std::cerr << CYAN << e.what() << RESET << std::endl;
#endif
		exit(-1);
	}
}

int main(int ac, char **av) //TODO fix segfault on start of second server
{
	validateArguments(ac, av);
	std::string password = ac == 3 ? std::string(av[2]) : std::string();
	try {
		signal(SIGPIPE, SIG_IGN);
		Server server = Server(Server(std::stoi(av[1]), password));
		int listen_sock = server.createListenSocket(server.getPort());
		server.mainLoop(listen_sock);
	}
	catch (std::exception &e) {
		std::cerr << "An error occurred" << std::endl;
		#ifdef MORE_INFO
		std::cerr << CYAN << e.what() << RESET << std::endl;
		#endif
		exit(-1);
	}
	return 0;
}