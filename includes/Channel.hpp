#ifndef MY_IRC_CHANNEL_HPP
#define MY_IRC_CHANNEL_HPP

#include <vector>
#include "User.hpp"
#include "my_irc.hpp"

class User;

class Channel {
public:
	Channel(const std::string &);
	~Channel();
	Channel();
	std::vector<User *>	getUsers() const;
	void				removeUser(User *);
	void				addUser(User &);
	void				setOper(User *);
	const std::string	&getName() const;
	std::string			getNameLowercased();
	void				setName(const std::string &name);
	bool				isOperator(User *);
	User				*findUserByNick(const std::string &nick);
private:
	std::string			_name;
	std::vector<User *>	_users;
	std::vector<User *>	_operators;
public:
	const std::vector<User *> &getOperators() const;

private:
	Channel & operator= (const Channel &other);
};
#endif //MY_IRC_CHANNEL_HPP