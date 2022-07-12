#include "../includes/Server.hpp"

Server::Server(int port, std::string password) : _port(port), _password(password), _serverName(SERVERNAME), _botName(BOTNAME) {
}

Server::Server() : _port(-1), _password(std::string()) {
}

int Server::createListenSocket(int port)
{
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if( sock < 0 )
		logger.logMessage("Socket fail", ERROR);
	int opt = 1;
	if (fcntl(sock, F_SETFL, O_NONBLOCK)) {
		logger.logMessage("Could not set non-blocking socket", ERROR);
		exit(-1);
	}
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	local.sin_port = htons(port);

	if( bind(sock,(struct sockaddr *)&local,sizeof(local)) < 0 ) {
		logger.logMessage("bind fail on port " + std::to_string(port), ERROR);
		exit(-1);
	}

	if( listen(sock,5) < 0 ) {
		logger.logMessage("listen fail on port " + std::to_string(port), ERROR);
		exit(-1);
	}
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
		logger.logMessage("_user fd undefined", ERROR);
		throw (FtException());
	}
	return user;
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
				logger.logMessage("poll returned -1, errno = " + std::string(strerror(errno)), ERROR);
				continue;
			default:
				processConnection(listen_sock);
				break;
			}
		}
}

Server::~Server() {
}

std::string Server::getPassword() const {
	return this->_password;
}

int Server::getPort() const {
	return this->_port;
}

Server &Server::operator=(const Server &other) {
	this->_port = other._port;
	this->_password = other._password;
	return(*this);
}