#ifndef MY_IRC_CHANNEL_HPP
#define MY_IRC_CHANNEL_HPP

#include <vector>
#include "User.hpp"

class Channel {
public:
	std::vector<User *> getUsers() const;
	~Channel();
	Channel();
private:
	std::vector<User *> _users;
	Channel & operator= (const Channel &other);
};
#endif //MY_IRC_CHANNEL_HPP
