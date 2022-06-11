#include "../includes/Channel.hpp"

Channel::Channel(const std::string &name) : _name(name) {
}

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

const std::string &Channel::getName() const {
	return _name;
}

void Channel::setName(const std::string &name) {
	_name = name;
}

std::string Channel::getNameLowercased() {
	return toLowercase(_name);
}

void Channel::removeUser(User *user) {
	std::vector<User *>::iterator toRemove = std::find(_users.begin(), _users.end(), user);
	_users.erase(toRemove);
}

void Channel::addUser(User &user) {
	_users.push_back(&user);
}

void Channel::setOper(User *user) {
	_operators.push_back(user);
}

const std::vector<User *> &Channel::getOperators() const {
	return _operators;
}

bool Channel::isOperator(User *user) {
	std::vector<User *>::iterator it = std::find(_operators.begin(), _operators.end(), user);
	if (it != _operators.end())
		return true;
	return false;
}

User *Channel::findUserByNick(const std::string &nick) {
	std::vector<User *>::iterator it = _users.begin();
	while (it != _users.end()) {
		if ((*it)->getNick() == nick) {
			return (*it);
		}
	}
	return 0;
}