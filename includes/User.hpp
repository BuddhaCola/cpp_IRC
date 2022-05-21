#ifndef MY_IRC_USER_HPP
#define MY_IRC_USER_HPP

#include <string>
#include <vector>

class User {
public:
	User();
	User(int);
	int 			getFd() const;
	std::string		getNick() const;
	void 			setNick(std::string nick);
	bool			isAuthorized() const;
	void			setAuthorized(bool authorized);
	User &			operator= (const User &other);
	std::ostream&	operator<<(std::ostream &);
	friend std::ostream& operator<< (std::ostream&, const User&);
	~User();
private:
	int			_fd;
	bool		_authorized;
	std::string	_nick;
	std::string	_username;
	std::string	_hostname;
	std::string	_realname;
public:
	const std::string &getUsername() const;

	void setUsername(const std::string &username);

	const std::string &getRealname() const;

	void setRealname(const std::string &realname);
};

#endif //MY_IRC_USER_HPP
