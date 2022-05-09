#ifndef MY_IRC_SERVER_HPP
#define MY_IRC_SERVER_HPP

#include <string>
#include <iostream>
#include "User.hpp"
#include "Channel.hpp"
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>

class Server {
public:
	Server(int, std::string);

	int 		creat_listen_socket(int);
	void		startLoop(int);
	std::string	getPassword() const;
	int 		getPort() const;

	Server & operator= (const Server &other);
	~Server();

private:
	int						_port;
	std::string				_password;
	std::vector<User *>		_users;
	std::vector<Channel *> 	_channels;
	Server();
};

#endif //MY_IRC_SERVER_HPP
