#include <sstream>
#include "../includes/Command.hpp"
#include "../includes/my_irc.hpp"

CommandEnum verbToCommand(std::string &verb) {
	return CommandEnum(std::find(CommandNames, CommandNames + NumCommands,  verb) - CommandNames);
}

std::string command_to_name(CommandEnum command)
{
	return (command < NumCommands) ? CommandNames[command] : "";
}

Command::Command(std::string &string, User &user) : _user(user) {
	std::string				current;

	std::stringstream		stream(string);
	stream >> current; //TODO test
	if (current.empty()) { //empty command
		return;
	}
	_type = verbToCommand(current);
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
	this->_textPart = &_arguments.at(_arguments.size() - 1); //bullshit
	return (*this);
}

Command::Command(const Command &other) : _type(other.getType()), _user(other.getUser()), _arguments(other._arguments) {
	this->_textPart = &_arguments.at(_arguments.size() - 1); //bullshit
}

CommandEnum Command::getType() const {
	return this->_type;
}

std::string Command::getTextPart() const {
	return *this->_textPart;
}

std::ostream &Command::operator<<(std::ostream &os) {
	os << "Command: " << command_to_name(this->_type);
	for (std::vector<std::string>::iterator it = this->_arguments.begin(); it != this->_arguments.end(); ++it) {
		os << '\n' << (*it);
	}
	return os;
}

std::ostream &operator<<(std::ostream &os, const Command &command) {
	std::vector<std::string> const &arguments = command.getArguments();
	int i = 0;
	os << "command: " << command_to_name(command._type) << "\nbody: " << command._textPart;
	for (int j = 0; j < arguments.size(); ++j) {
		os << '\n' << arguments.at(j);
	}
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

void Command::setArguments(const std::vector<std::string> &arguments) {
	_arguments = arguments;
}

void Command::parseTextPart(std::stringstream &stream) {
	std::string textPart;
	if (std::getline(stream, textPart))
		*_textPart = _textPart->append(textPart);
}
