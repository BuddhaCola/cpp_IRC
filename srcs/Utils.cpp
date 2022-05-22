#include <string>
#include <vector>
#include "../includes/my_irc.hpp"

bool	allowedCharacter(char c) {
	if ((c >= 65 && c <= 90) /*'A'-'Z'*/ || (c >= 97 && c <= 123) /*'a'-'z'*/ || (c >= 48 && c <= 57) /*'0'-'9'*/ || (c == '-' || c == '_'))
		return true;
	return false;
}

bool validateString(std::string &string) {
	for (std::vector<char>::iterator it = string.begin(); it != string.end(); ++it)
		if (!allowedCharacter(*it))
			return false;
	return true;
}

std::string	trim(const std::string& string) {
	const char*	whitespace = " \t\n\r\f\v";
	size_t begin = string.find_first_not_of(whitespace);
	if (begin == std::string::npos) {
		return std::string();
	}
	size_t end = string.find_last_not_of(whitespace);
	return std::string(string.substr(begin, end - begin + 1));
}

std::string	getmessage(std::string &string) {
	return (std::string(string.substr(string.find(':'), string.length())));
}

std::string toLowercase(std::string &original) {
	std::string lowercased;
	for (std::vector<char>::iterator it = original.begin(); it != original.end(); ++it) {
		lowercased += std::tolower(*it);
	}
	return lowercased;
}

std::string	currentTime() {
	std::time_t result = std::time(nullptr);
	return std::string(std::asctime(std::localtime(&result)));
}

