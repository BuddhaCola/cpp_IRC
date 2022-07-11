#ifndef MY_IRC_LOGGER_HPP
#define MY_IRC_LOGGER_HPP

#include <sstream>
#include <iostream>
#include <fstream>
#include <iostream>

#include "User.hpp"
#include "My_irc.hpp"

#define MODE std::fstream::in | std::fstream::out | std::fstream::app

enum LogType {
	IN,
	OUT,
	INFO,
	ERROR,
	DEV
};

class Logger {
public:
	Logger();
	void logMessage(std::stringstream &message, LogType type);
	void logMessage(std::string message, LogType type);
	void logUserMessage(std::string message, User &user, LogType type);
	Logger & operator= (const Logger &other);
	Logger(Logger const &);
	~Logger();
private:
	std::fstream	file;
	std::string		filename;
};
#endif //MY_IRC_LOGGER_HPP
