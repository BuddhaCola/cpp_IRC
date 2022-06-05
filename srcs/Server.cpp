#include "../includes/Server.hpp"
#include "../includes/Command.hpp"
#include "../includes/User.hpp"

Server::Server(int port, std::string password) : _port(port), _password(password){
}

Server::Server() : _port(-1), _password(std::string()) {
}

int Server::creatListenSocket(int port)
{
	int sock = socket(AF_INET,SOCK_STREAM,0);//The second parameter here represents TCP
	if( sock < 0 )
		throw("socket fail...\n");

	int opt = 1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = htonl(INADDR_ANY);// Address of any type
	local.sin_port = htons(port);

	if( bind(sock,(struct sockaddr *)&local,sizeof(local)) < 0 )
		throw("bind fail...\n");

	if( listen(sock,5) < 0 )
		throw("listen fail...\n");
	return sock;
}

void Server::StartLogMessage() {
	std::stringstream logMessageStream;

	logMessageStream << "Starting server on port " << this->_port << ' ';
	logMessageStream << "Password is " << (getPassword().empty() ? "not set" :
										   ("set to |" + getPassword()) + "|")
					 << std::endl;
	logger.logMessage(logMessageStream, INFO);
}

void Server::createFdList(int listen_socket)
{
	int i = 0;

	for (; i < MAX_USERS; i++) {
		fd_list[i].fd = -1;// File descriptor
		fd_list[i].events = 0;// Set of events to monitor
		fd_list[i].revents = 0;// Ready Event Set of Concerned Descriptors
	}
	i = 0;
	for (; i < MAX_USERS; i++) {
		if (fd_list[i].fd == -1)
		{
			fd_list[i].fd = listen_socket;
			fd_list[i].events = POLLIN;// Concern about Read-Only Events
			break;
		}
	}
}

User * Server::checkFdUser(int i)
{
	std::stringstream logStream;
	User *user = 0;
	for (std::vector<User *>::iterator it = this->_users.begin();
		 it != this->_users.end(); ++it)
	{
		if ((*it)->getFd() == fd_list[i].fd)
		{
			user = *(it);
			break;
		}
	}
	if (!user)
	{
		logStream << "_user fd undefined" << std::endl;
		logger.logMessage(logStream, ERROR);
		throw (FtException());
	}
	return user;
}

void Server::readFromBuffer(int i)
{
	std::stringstream logStream;

}

void Server::mainLoop(int listen_sock)
{
	StartLogMessage();
	createFdList(listen_sock);

	while (1)
	{
		switch (poll(fd_list, MAX_USERS, POLL_TIMEOUT))
		{
			case 0:
				pingClient();
				continue;
			case -1:
				std::cerr << "poll fail..." << std::endl;
				continue;
			default:
				pollDefault(listen_sock);
				break;
			}
		}
}

Server::~Server() {
}

std::string Server::getPassword() const {
	return this->_password;
}

int Server::getPort() const
{
	return this->_port;
}

Server &Server::operator=(const Server &other) {
	this->_port = other._port;
	this->_password = other._password;
	return(*this);
}

void Server::ServerMessageToUser(Command const &command)
{
	std::stringstream toSend;
	Response 	const &response 	= command.getResponse();
	User 		const &user 		= command.getUser();

//	:lala!oper@178.204.221.161 PRIVMSG lala :hi
//	:irc.ircnet.su 001 LALA :Welcome to the IrcNet.ru IRC Network LALA!JOPA@178.204.221.161
//	:wow!lol@188.242.23.62 PRIVMSG lala :вот такая вот херня
	toSend << ':' << "Megaserver";
	if (response.code)
		toSend << ' ' << response.code;

	toSend << ' ' << user.getNick() << ' ' << ':' << response.body;

}







