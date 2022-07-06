#include "../../includes/Server.hpp"

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
	}
}