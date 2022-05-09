#include "../includes/Channel.hpp"

Channel::Channel() {
}

Channel::~Channel() {
}

Channel &Channel::operator=(const Channel &other) {
	this->_users = other.getUsers();
	return(*this);
}

std::vector<User *> Channel::getUsers() const {
	return _users;
}
