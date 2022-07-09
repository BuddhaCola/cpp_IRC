#ifndef MY_IRC_USER_HPP
#define MY_IRC_USER_HPP

#include <string>
#include <vector>
#include <ctime>
#include "Channel.hpp"

class Channel;

class User {
public:
	User();
	User(int);
	int 					getFd() const;
	std::string				getNick() const;
	void 					setNick(std::string nick);
	bool					isAuthorized() const;
	void					setAuthorized(bool authorized);
	bool					isRegistered() const;
	void					setRegistered(bool registered);
	bool					isOper() const;
	void					setOper(bool oper);
	User &					operator= (const User &other);
	std::ostream&			operator<<(std::ostream &);
	friend std::ostream&	operator<< (std::ostream&, const User&);
	~User();
private:
	std::string				_nick;
	std::string				_nickLowercase;
	int						_fd;
	bool					_authorized;
	bool					_registered;
	bool					_oper;

private:
	std::time_t 			_timestamp;
	std::string				_username;
	std::string				_realname;
	std::string				_IP;
	int 					_port;
	std::vector<Channel *>	_channels;

public:
	const std::string		&getNickLowercase() const;
	void					setNickLowercase(const std::string &nickLowercase);
	const					std::string &getIp() const;
	void					setIp(const std::string &ip);
	int						getPort() const;
	void					setPort(int port);
	std::vector<Channel *>	&getChannels();
	const std::string		&getUsername() const;
	void					setUsername(const std::string &username);
	const std::string		&getRealname() const;
	void					setRealname(const std::string &realname);
	std::string				getUserInfoString() const;
	time_t					getTimestamp() const;
	void					setTimestamp(time_t timestamp);
	void					addChannel(Channel *);
	void					removeChannel(Channel *channel);
};

#endif //MY_IRC_USER_HPP
