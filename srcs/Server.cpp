#include <sstream>
#include "../includes/Server.hpp"

Server::Server(int port, std::string password) : _port(port), _password(password){
}

Server::Server() : _port(-1), _password(std::string()) {
}

int Server::creat_listen_socket(int port)
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

//	int num = sizeof(fd_list) / sizeof(fd_list[0]);
	int i = 0;

	for (; i < MAX_USERS; i++)
	{
		fd_list[i].fd = -1;// File descriptor
		fd_list[i].events = 0;// Set of events to monitor
		fd_list[i].revents = 0;// Ready Event Set of Concerned Descriptors
	}
	// 3. Add read-only events for file descriptors of interest

	i = 0;
	for (; i < MAX_USERS; i++)
	{
		if (fd_list[i].fd == -1)
		{
			fd_list[i].fd = listen_socket;
			fd_list[i].events = POLLIN;// Concern about Read-Only Events
			break;
		}
	}
}


void Server::startLoop(int listen_sock)
{
	std::stringstream logStream;
	StartLogMessage();
	createFdList(listen_sock);

	while (1)
	{
		//4. Start calling poll and wait for the file descriptor set of interest to be ready
		switch (poll(fd_list, MAX_USERS, 3000))
		{
			case 0:// The state of the denominator has exceeded before it has changed. timeout Time
				continue;
			case -1:// failed
				logStream << "poll fail..." << std::endl;
				logger.logMessage(logStream, ERROR);
				continue;
			default: // Succeed
			{
				//   If it is a listener file descriptor, call accept to accept a new connection
				//   If it is a normal file descriptor, read is called to read the data
				int i = 0;
				for (; i < MAX_USERS; i++)
				{
					if (fd_list[i].fd == -1)
					{
						continue;
					}
					if (fd_list[i].fd == listen_sock &&
						(fd_list[i].revents & POLLIN))
					{
						// 1. Provide a connection acceptance service if the listening socket is ready to read

						struct sockaddr_in client;
						socklen_t len = sizeof(client);
						int new_sock = accept(listen_sock,
											  (struct sockaddr *) &client,
											  &len);
						if (new_sock < 0)
						{
							logStream << "accept fail..." << std::endl;
							logger.logMessage(logStream, ERROR);
							continue;
						}
						//After obtaining the new file descriptor, add the file descriptor to the array for the next time you care about the file descriptor
						int i = 0;
						for (; i < MAX_USERS; i++)
						{
							if (fd_list[i].fd ==
								-1)//Place the first value in the array at - 1
								break;
						}
						if (i < MAX_USERS)
						{
							fd_list[i].fd = new_sock;
							fd_list[i].events = POLLIN;
						} else
						{
							close(new_sock);
						}
						User *new_user = new User(fd_list[i].fd);
						this->getPassword().empty() ? new_user->setAuthorized(true) : new_user->setAuthorized(false);
						logStream << "get a new link " <<
							   inet_ntoa(client.sin_addr) << ":" <<
							   ntohs(client.sin_port) << std::endl;
						new_user->setPort(ntohs(client.sin_port)); //Ваня, добавь к себе такую же штуку
						new_user->setIp(inet_ntoa(client.sin_addr));
						int iq = ntohs(client.sin_port);
						logger.logMessage(logStream, INFO);
						_users.push_back(new_user);
//						std::cout << "Created " << i <<  " _user" << std::endl;
//						for (size_t i = 0; i < _users.size(); ++i) {
//							std::cout << "fd:  " <<
//							_users[i]->getFd()
//							<< std::endl;
//						}
						continue;
					}

					//2. At this point, we are concerned with ordinary file descriptors.
					//   Provide services to read data at this time
					if (fd_list[i].revents & POLLIN)
					{
						char buf[1024];
						ssize_t s = read(fd_list[i].fd, buf, sizeof(buf) - 1);
						if (s < 0)
						{
							logStream << "read fail..." << std::endl;
							logger.logMessage(logStream, ERROR);
							continue;
						} else if (s == 0)
						{
							logStream << "client quit..." << std::endl;
							logger.logMessage(logStream, INFO);
							close(fd_list[i].fd);
							fd_list[i].fd = -1;
						} else
						{
							buf[s] = 0;
							User *user = 0;
							for (std::vector<User*>::iterator it = this->_users.begin(); it != this->_users.end(); ++it) {
								if ((*it)->getFd() == fd_list[i].fd) {
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

void Server::sendErrorToUser(Command const &)
{

}