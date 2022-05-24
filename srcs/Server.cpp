#include <sstream>
#include "../includes/Server.hpp"

Server::Server(int port, std::string password) : _port(port), _password(password){
}

Server::Server() : _port(-1), _password(std::string()) {
}

void Server::creat_listen_socket(int port)
{
	int sock = socket(AF_INET,SOCK_STREAM,0);//The second parameter here represents TCP
	if( sock < 0 )
		throw("socket fail...\n");

	int opt = 1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = htonl(INADDR_ANY);// Address of any _type
	local.sin_port = htons(port);

	if( bind(sock,(struct sockaddr *)&local,sizeof(local)) < 0 )
		throw("bind fail...\n");

	if( listen(sock,5) < 0 )
		throw("listen fail...\n");
	_listen_socket = sock;
}

void Server::addNewUser(int fd)
{
	std::stringstream logStream;

	User *new_user = new User(fd);
	this->getPassword().empty() ? new_user->setAuthorized(true)
								: new_user->setAuthorized(false);
	logStream << "get a new link " <<
			  inet_ntoa(_client.sin_addr) << ":" <<
			  ntohs(_client.sin_port) << std::endl;
	_users.push_back(new_user);
//	std::cout << "Created " << i <<  " _user" << std::endl;
//						for (size_t i = 0; i < _users.size(); ++i) {
//							std::cout << "fd:  " <<
//							_users[i]->getFd()
//							<< std::endl;
}

int Server::create_new_socket()
{
	socklen_t len = sizeof(_client);
	int new_sock = accept(_listen_socket,(struct sockaddr *) &_client,
						  &len);
	return new_sock;
}

void Server::StartLogMessage() {
	std::stringstream logMessageStream;

	logMessageStream << "Starting server on port " << this->_port << ' ';
	logMessageStream << "Password is " << (getPassword().empty() ? "not set" :
										   ("set to |" + getPassword()) + "|")
					 << std::endl;
	logger.logMessage(logMessageStream, INFO);
}

void Server::startLoop()
{
	std::stringstream logStream;
	StartLogMessage();

	_num = sizeof(_fd_list) / sizeof(_fd_list[0]);
	int i = 0;

	for (; i < _num; i++)
	{
		_fd_list[i].fd = -1;// File descriptor
		_fd_list[i].events = 0;// Set of events to monitor
		_fd_list[i].revents = 0;// Ready Event Set of Concerned Descriptors
	}
	// 3. Add read-only events for file descriptors of interest

	i = 0;
	for (; i < _num; i++)
	{
		if (_fd_list[i].fd == -1)
		{
			_fd_list[i].fd = _listen_socket;
			_fd_list[i].events = POLLIN;// Concern about Read-Only Events
			break;
		}
	}


	while (1)
	{
		//4. Start calling poll and wait for the file descriptor set of interest to be ready
		switch (poll(_fd_list, _num, 3000))
		{
			case 0:// The state of the denominator has exceeded before it has changed. timeout Time
				continue;
			case -1:// failed
				logStream << "poll fail..." << std::endl;
				logger.logMessage(logStream, ERROR);
				continue;
			default:
				poll_procces();// Succeed
		}
	}
}

void Server::poll_procces()
{
	std::stringstream logStream;


	_num = sizeof(_fd_list)/sizeof(_fd_list[0]);
	int i = 0;
	for (; i < _num; i++)
	{
		if (_fd_list[i].fd == -1)
		{
			continue;
		}
		if (_fd_list[i].fd == _listen_socket &&
			(_fd_list[i].revents & POLLIN))
		{
			int new_sock = create_new_socket();
			if (new_sock < 0)
			{
				logStream << "accept fail..." << std::endl;
				continue;
			}
			//After obtaining the new file descriptor, add the file descriptor to the array for the next time you care about the file descriptor
			int i = 0;
			for (; i < _num; i++)
			{
				if (_fd_list[i].fd == -1)//Place the first value in the array
					// at - 1
					break;
			}
			if (i < _num)
			{
				_fd_list[i].fd = new_sock;
				_fd_list[i].events = POLLIN;
			} else
			{
				close(new_sock);
			}
			addNewUser(_fd_list[i].fd);
			continue;
		}
		if (_fd_list[i].revents & POLLIN)
		{
			char buf[1024];
			ssize_t s = read(_fd_list[i].fd, buf, sizeof(buf) - 1);
			if (s < 0)
			{
				logStream << "read fail..." << std::endl;
				logger.logMessage(logStream, ERROR);
				continue;
			} else if (s == 0)
			{
				logStream << "client quit..." << std::endl;
				close(_fd_list[i].fd);
				_fd_list[i].fd = -1;
			} else
			{
				buf[s] = 0;
				User *user = 0;
				for (std::vector<User*>::iterator it = this->_users.begin(); it != this->_users.end(); ++it) {
					if ((*it)->getFd() == _fd_list[i].fd) {
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
				handleRequest(buf, *(user));
			}
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
