#include "../includes/User.hpp"

User::User() : _fd(-1), _nick(std::string()) {
}

User::User(int fd_user) : _fd(fd_user), _nick(std::string()) {
}

User::~User() {
}

int User::getFd() const {
	return this->_fd;
}

std::string User::getNick() const {
	return this->_nick;
}

void User::setNick(std::string nick) {
	this->_nick = nick;
}

User &User::operator=(const User &other) {
	this->_fd = other.getFd();
	this->_nick = other.getNick();
	return (*this);
}
