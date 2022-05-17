#include "../includes/User.hpp"

User::User() : _fd(-1), _nick(std::string()), _authorized(false) {
}

User::User(int fd_user) : _fd(fd_user), _nick(std::string()), _authorized(false){
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

bool User::isAuthorized() const {
	return _authorized;
}

void User::setAuthorized(bool authorized) {
	_authorized = authorized;
}
