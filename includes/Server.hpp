
#ifndef MY_IRC_SERVER_HPP
#define MY_IRC_SERVER_HPP

#include <string>
#include <iostream>
#include "User.hpp"
#include "Channel.hpp"

class Server {
public:
	Server(int, std::string);

	void		startLoop();
	std::string	getPassword() const;

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
