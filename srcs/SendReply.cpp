//
// Created by Whistler Archangel on 6/10/22.
//
#include "../includes/allAnswers.hpp"
#include "../includes/Server.hpp"

void		Server::sendReply(Command const &command, int replyCode)
{
	std::string	msg = ":My_IRC ";
	std::stringstream	ss;
	User &user = command.getUser();
	std::string commandName = command.typeToString();

	ss << replyCode;
	msg += ss.str() + " " + user.getNick();
	switch (replyCode)
	{
		case RPL_NOWAWAY:
			msg += " :You have been marked as being away\n";
			break;
		case RPL_WHOISOPERATOR:
			msg +=  + " :is an IRC operator\n";
			break;
		case RPL_ENDOFWHOWAS:
			msg +=  + " :End of WHOWAS\n";
			break;
		case RPL_LISTSTART:
			msg += "Channel :Users  Name\n";
			break;
		case RPL_NOTOPIC:
			msg +=  + " :No topic is set\n";
			break;
		case RPL_SUMMONING:
			msg +=  + " :Summoning user to IRC\n";
			break;
		case RPL_ENDOFWHO:
			msg += " " + command.getArgument(0) + " :End of /WHO list\n";
			break;
		case RPL_ENDOFNAMES:
			msg +=  + " :End of /NAMES list\n";
			break;
		case RPL_ENDOFLINKS:
			msg +=  + " :End of /LINKS list\n";
			break;
		case RPL_ENDOFBANLIST:
			msg +=  + " :End of channel ban list\n";
			break;
		case RPL_ENDOFINFO:
			msg += " :End of /INFO list\n";
			break;
		case RPL_MOTDSTART:
			msg += " :- Message of the day - \n";
			break;
		case RPL_ENDOFMOTD:
			msg += " :End of /MOTD command\n";
			break;
		case RPL_YOUREOPER:
			msg += " " + commandName + " :You are now an IRC operator\n";
			break;
		case RPL_REHASHING:
			msg +=  + " :Rehashing\n";
			break;
		case RPL_USERSSTART:
			msg += " :UserID   Terminal  Host\n";
			break;
		case RPL_USERS:
			msg += " :%-8s %-9s %-8s\n"; // Хз, что это UPD: Понял, строка длиной 8 символов
			break;
		case RPL_ENDOFUSERS:
			msg += " :End of users\n";
			break;
		case RPL_NOUSERS:
			msg += " :Nobody logged in\n";
			break;
		case RPL_ENDOFSTATS:
			msg +=  + " :End of /STATS report\n";
			break;
		case RPL_STATSUPTIME:
			msg += " :Server Up %d days %d:%02d:%02d\n"; // Поменять
			break;
		case RPL_UMODEIS:
			msg +=  + "\n";
			break;
		case RPL_LUSEROP:
			msg +=  + " :operator(s) online\n";
			break;
		case RPL_LUSERUNKNOWN:
			msg +=  + " :unknown connection(s)\n";
			break;
		case RPL_LUSERCHANNELS:
			msg +=  + " :channels formed\n";
			break;
		case RPL_LUSERME:
			msg += " :I have clients and servers\n";
			break;
		case RPL_ADMINME:
			msg +=  + " :Administrative info\n";
			break;
		default:
			msg += "UNKNOWN REPLY\n";
			break;
	}
	logger.logUserMessage(msg, user, OUT);
	send(user.getFd(), msg.c_str(), msg.size(), 0);
}