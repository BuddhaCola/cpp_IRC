#ifndef MY_IRC_USER_HPP
#define MY_IRC_USER_HPP

#include <string>
#include <vector>

class User {
public:
	User();
	User(int);
	int 		getFd() const;
	std::string	getNick() const;
	void 		setNick(std::string nick);
	User & operator= (const User &other);
	~User();
private:
	int 		_fd;
	std::string	_nick;
};

#endif //MY_IRC_USER_HPP
