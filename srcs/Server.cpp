#include "../includes/Server.hpp"

Server::Server(int port, std::string password) : _port(port), _password(password){
}

Server::Server() : _port(-1), _password(std::string()) {
}

void Server::startLoop() {
	std::cout << "Starting server on port " << this->_port << std::endl;
	std::cout << "Password is " << (getPassword().empty() ? "not set" : ("set to |" + getPassword()) + "|") << std::endl;
	//TODO
	while (1) {
		//TODO
	}
}

Server::~Server() {
}

std::string Server::getPassword() const {
	return this->_password;
}

Server &Server::operator=(const Server &other) {
	this->_port = other._port;
	this->_password = other._password;
	return(*this);
}
