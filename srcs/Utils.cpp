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

// |Username * localhost :Realname
std::vector<std::string> splitString(std::string string) {
	const char*	whitespace = " \t\n\r\f\v";
	std::vector<std::string> vector;
// бля, я туплю. Потом доделаю.
//	size_t found = 0;
//	size_t current = string.find_first_not_of(whitespace);
//
//	while (current != std::string::npos) {
//		found = string.find_first_of(whitespace);
//		vector.push_back(string.substr(current, found));
//	}
	return vector;
}

