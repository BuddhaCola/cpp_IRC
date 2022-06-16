#include <sstream>
#include "../includes/Command.hpp"
#include "../includes/my_irc.hpp"

CommandEnum verbToCommand(std::string &verb) {
	return CommandEnum(std::find(CommandNames, CommandNames + UNDEFINED, verb) - CommandNames);
}

std::string command_to_name(CommandEnum command) {
	return (command < UNDEFINED) ? CommandNames[command] : "";
}

Command::Command(std::string &string, User &user) : _user(user) {
	std::string				current;
	std::stringstream		stream(string);

	_textPart = 0;
	stream >> current; //TODO test
	if (current.empty()) { //empty command
		return;
	}
	current = toUppercase(current);
	_type = verbToCommand(current);
	if (_type == UNDEFINED) {
		throw FtException();
	}
	while (stream >> current) {
		if (current.at(0) == ':') {
			_arguments.push_back(std::string(current, 1));
			_textPart = &_arguments.at(_arguments.size() - 1);
			parseTextPart(stream);
			break;
		}
		_arguments.push_back(std::string(current));
	}
}

Command::~Command() {
}

Command &Command::operator=(const Command &other) {
	this->_type = other._type;
	this->_arguments = other._arguments;
	if (other._textPart) {
		_textPart = &_arguments.at(_arguments.size() - 1);
	}
	return (*this);
}

Command::Command(const Command &other) : _type(other.getType()), _user(other.getUser()), _arguments(other._arguments) {
	if (other._textPart) {
		_textPart = &_arguments.at(_arguments.size() - 1);
	}
}

CommandEnum Command::getType() const {
	return this->_type;
}

std::string Command::getTextPart() const {
	return *this->_textPart;
}

std::ostream &Command::operator <<(std::ostream &os) const {
	os << "Command: " << this->typeToString();
	os << argumentsToString();
	return os;
}

std::ostream &operator<<(std::ostream &os, const Command &command) {
	os << "Command: " << command.typeToString();
	os << command.argumentsToString();
	return os;
}

User &Command::getUser() const {
	return this->_user;
}

const std::vector<std::string> &Command::getArguments() const {
	return _arguments;
}

const std::string &Command::getArgument(int i) const {
	return _arguments.at(i);
}

const Response &Command::getResponse() const
{
	return _response;
}

void Command::setArguments(const std::vector<std::string> &arguments) {
	_arguments = arguments;
}

void Command::parseTextPart(std::stringstream &stream) {
	std::string textPart;
	if (std::getline(stream, textPart))
		*_textPart = _textPart->append(textPart);
}

std::string Command::typeToString() const {
	return CommandNames[_type];
}

std::string Command::argumentsToString() const {
	std::stringstream os;
	for (std::vector<std::string const>::iterator it = this->_arguments.begin(); it != this->_arguments.end(); ++it) {
		os << (*it);
		if (it + 1 != this->_arguments.end())
			os << '\n';
	}
	return os.str();
}


