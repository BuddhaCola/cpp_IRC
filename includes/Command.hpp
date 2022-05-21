#ifndef MY_IRC_COMMAND_HPP
#define MY_IRC_COMMAND_HPP

#include <string>
#include <iostream>
#include "User.hpp"

#define COMMANDS(X) \
X(PASS),\
X(USER),\
X(NICK),\
X(PONG),\
X(PRIVMSG),\
X(NOTICE),\
X(JOIN),\
X(OPER),\
X(QUIT),\
X(KILL),\
X(KICK),\
X(PING),\
X(LIST),\
X(WHO)

//enum representation
#define X(e) e
enum CommandEnum { COMMANDS(X), NumCommands };
#undef X

//string representation
#define X(s) #s
static const std::string CommandNames[] = { COMMANDS(X) };
#undef X

class Command {
public:
	Command(std::string &string, User &user);
	CommandEnum 					getType() const;
	std::string						getTextPart() const;
	User							&getUser() const;
	const std::vector<std::string>	&getArguments() const;
	const std::string				&getArgument(int) const;
	void							setArguments(const std::vector<std::string> &arguments);

	~Command();
	Command &	operator=	(const Command &);
	std::ostream& operator<<(std::ostream &);
	friend std::ostream& operator<< (std::ostream&, const Command&);
	Command(Command const &);
private:
	CommandEnum 				_type;
	std::string					*_textPart;
	std::vector<std::string>	_arguments;
	User						&_user;
};

#endif //MY_IRC_COMMAND_HPP
