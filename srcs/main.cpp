#include <iostream>
#include "../includes/my_irc.hpp"
#include "../includes/Server.hpp"
#include <stdlib.h>

void validateArguments(int ac, char **av) {
	if (ac < 2) {
		std::cout << "Usage: ./ircserv <port> <handlePassword>" << std::endl;
		std::cout << "too low arguments!" << std::endl;
		exit(-1);
	}
	if (ac > 2) {
		if (ac > 3) {
			std::cout << "Usage: ./ircserv <port> <handlePassword>" << std::endl;
			std::cerr << "Too many arguments!" << std::endl;
			exit(-1);
		}
		if (strlen(av[2]) == 0) {
			std::cerr << "handlePassword can't be an empty string" << std::endl;
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

int main(int ac, char **av)
{
	validateArguments(ac, av);
	std::string password = ac == 3 ? std::string(av[2]) : std::string();
	try {
		Server server = Server(Server(std::stoi(av[1]), password));
		int listen_sock = server.creat_listen_socket(server.getPort());
		server.startLoop(listen_sock);
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