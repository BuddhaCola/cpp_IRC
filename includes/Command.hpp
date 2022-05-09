#ifndef MY_IRC_COMMAND_HPP
#define MY_IRC_COMMAND_HPP

#include <string>
#include <iostream>

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
	Command(const std::string& string);
	CommandEnum getType();
	std::string	getBody();

	~Command();
	Command &	operator=	(const Command &other);
	std::ostream& operator<<(std::ostream& os);
	friend std::ostream& operator<< (std::ostream& out, const Command& point);
	Command(Command const &);
private:
	CommandEnum type;
	std::string	body;
};

#endif //MY_IRC_COMMAND_HPP
