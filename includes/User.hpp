#ifndef MY_IRC_USER_HPP
#define MY_IRC_USER_HPP

#include <string>
#include <vector>

class User {
public:
	User();
	User(int);
	int 					getFd() const;
	std::string				getNick() const;
	void 					setNick(std::string nick);
	bool					isAuthorized() const;
	void					setAuthorized(bool authorized);
	User &					operator= (const User &other);
	std::ostream&			operator<<(std::ostream &);
	friend std::ostream&	operator<< (std::ostream&, const User&);
	~User();
private:
	int			_fd;
	bool		_authorized;
	std::string	_nick;
	std::string	_nickLowercase;

private:
	std::string	_username;
	std::string	_hostname;
	std::string	_realname;
	std::string	_IP;
	int 		_port;
public:
	const std::string		&getNickLowercase() const;
	void					setNickLowercase(const std::string &nickLowercase);
	const					std::string &getIp() const;
	void					setIp(const std::string &ip);
	int						getPort() const;
	void					setPort(int port);
	const std::string		&getUsername() const;
	void					setUsername(const std::string &username);
	const std::string		&getRealname() const;
	void					setRealname(const std::string &realname);
	std::string				getUserInfoString();
};

#endif //MY_IRC_USER_HPP
