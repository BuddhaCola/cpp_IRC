#ifndef MY_IRC_MY_IRC_HPP
#define MY_IRC_MY_IRC_HPP

#define CYAN "\033[36m"
#define RESET "\033[0m"

class FtException: public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "Look for FtException";
	}
} ftex;

#endif //MY_IRC_MY_IRC_HPP
