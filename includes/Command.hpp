#ifndef MY_IRC_COMMAND_HPP
#define MY_IRC_COMMAND_HPP

#include <string>
#include <iostream>
#include <sstream>
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
enum CommandEnum { COMMANDS(X), UNDEFINED }; //rename UNDEFINED
#undef X

//string representation
#define X(s) #s
static const std::string CommandNames[] = { COMMANDS(X) };
#undef X

typedef struct Response {
	int 				code;
	std::stringstream 	body;
}	Response;

class Command {
public:
	Command(std::string &string, User &user);
	CommandEnum 					getType() const;
	std::string						getTextPart() const;
	std::string						typeToString() const;
	std::string						argumentsToString() const;
	User							&getUser() const;
	const std::vector<std::string>	&getArguments() const;
	const std::string				&getArgument(int) const;
	void							setArguments(const std::vector<std::string> &arguments);
	const Response &getResponse() const;

	~Command();
	Command &	operator=	(const Command &);
	std::ostream& operator<<(std::ostream &) const;
	friend std::ostream& operator<< (std::ostream&, const Command&);
	Command(Command const &);
private:


	CommandEnum 				_type;
	std::string					*_textPart;
	std::vector<std::string>	_arguments;
	User						&_user;
	Response					_response;


private:

	void 						parseTextPart(std::stringstream &);
};

#endif //MY_IRC_COMMAND_HPP
