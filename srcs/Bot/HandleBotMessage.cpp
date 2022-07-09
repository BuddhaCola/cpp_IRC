#include "../../includes/Server.hpp"

void Server::botListUsers(User &user){
	std::stringstream	stream;
	std::string			message;
	int					usersCount = _users.size();
	std::vector<User *>	registeredUsers;
	int					registeredUsersCount;

	for (int i = 0; i < usersCount - 1; ++i) {
		if (_users.at(i)->isRegistered()) {
			registeredUsers.push_back(_users.at(i));
		}
	}
	registeredUsersCount = registeredUsers.size();

	for (int i = 0; i <= registeredUsersCount - 1; ++i) {
		message += registeredUsers.at(i)->getNick();
		if (i < registeredUsersCount - 1)
			message += ", ";
	}
	stream << ':' + _botName << " " <<
		   "PRIVMSG" << " " << user.getNick() << " :" << "There's " << usersCount - 1 << " user" << (usersCount - 1 == 1 ? ' ' : 's') << " on the server right now. ";
	if (usersCount > 1)
		stream << registeredUsersCount << " of them " << (registeredUsersCount == 1 ? "is" : "are") << " registered";
	if (registeredUsersCount)
		stream << ":\n" << message;
	stream  << "\r\n";
	std::string toSend = stream.str();
	write(user.getFd(), toSend.c_str(), toSend.length());
	logger.logUserMessage(toSend, user, OUT);
}

void Server::botListChannels(User &user){
	std::stringstream	stream;
	std::string			message;
	int					channelsCount = _channels.size();

	for (int i = 0; i < channelsCount; ++i) {
		message +=_channels.at(i)->getName();
		if (i < channelsCount - 1)
			message += ", ";
	}
	stream << ':' + _botName << " " << "PRIVMSG" << " " << user.getNick() << " :" << "There's " << channelsCount << " channel" << (channelsCount == 1 ? ' ' : 's') <<" on the server right now";
	if (channelsCount) {
		stream << ":\n" << message;
	}
	stream << "\r\n";
	std::string toSend = stream.str();
	write(user.getFd(), toSend.c_str(), toSend.length());
	logger.logUserMessage(toSend, user, OUT);
}

void Server::botGreeting(User &user){
	std::stringstream	stream;
	std::string			message;

	message = "Hey, " + user.getNick() + "!\nI'm the helper bot!\nWelcome to IRC Server!";
	stream << ':' + _botName << " " <<
		   "PRIVMSG" << " " << user.getNick() << " :" << message << "\r\n";
	std::string toSend = stream.str();
	write(user.getFd(), toSend.c_str(), toSend.length());
	logger.logUserMessage(toSend, user, OUT);
	botListUsers(user);
	if (!_channels.empty()) {
		botListChannels(user);
	}
}

void Server::handleBotMessage(const Command &command) {
	User				&user = command.getUser();
	const std::string&	string = command.getArgument(command.getArguments().size() - 1);
	std::stringstream	stream(string);
	std::string			current;

	while (stream >> current) {
		std::string word = toLowercase(current);
		if (word == "hey" || word == "hi" || word == "hello"|| word == "привет") {
			std::stringstream toSend;
			toSend << ':' + _botName << " " <<
				   "PRIVMSG" << " " << user.getNick() << " :" << "Hey!" << "\r\n";
			std::string str = toSend.str();
			write(user.getFd(), str.c_str(), str.size());
			logger.logUserMessage(str, user, OUT);
		}
		else if (word == "list") {
			botListUsers(user);
		}
		else if (word == "channels") {
			botListChannels(user);
		}
	}
}