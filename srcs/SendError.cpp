//
// Created by Ivan on 01.06.2022.
//
#include "../includes/allAnswers.hpp"
#include "../includes/User.hpp"
#include "../includes/Server.hpp"


void Server::sendError(Command const &command , int errorCode )
{
	std::string	msg = ":My_IRC ";
	std::stringstream	ss;
	User &user = command.getUser();
	std::string commandName = command.typeToString();
	if (command.getArguments().size()) {
		std::vector<std::string> const &arguments = command.getArguments();
	}
	ss << errorCode;
	msg += ss.str() + " " + user.getNick();
	switch (errorCode)
	{
		case ERR_NOSUCHNICK:
			msg += ' ' + command.getArgument(0) + " :No such nick/channel\n";
			break;
		case ERR_NOSUCHSERVER:
			msg += " :No such server\n";
			break;
		case ERR_NOSUCHCHANNEL:
			msg += ' ' + command.getArgument(0) + " :No such channel\n";
			break;
		case ERR_CANNOTSENDTOCHAN:
			msg += " :Cannot send to channel\n";
			break;
		case ERR_TOOMANYCHANNELS:
			msg += " :You have joined too many channels\n";
			break;
		case ERR_WASNOSUCHNICK:
			msg += " :There was no such nickname\n";
			break;
		case ERR_TOOMANYTARGETS:
			msg += " :Duplicate recipients delivered\n";
			break;
		case ERR_NOORIGIN:
			msg += " :No origin specified\n";
			break;
		case ERR_NORECIPIENT:
			msg += " :No recipient given (" + commandName + ")\n";
			break;
		case ERR_NOTEXTTOSEND:
			msg += " :No text to send\n";
			break;
		case ERR_NOTOPLEVEL:
			msg += " " + commandName + " :No toplevel domain specified\n";
			break;
		case ERR_WILDTOPLEVEL:
			msg += " " + commandName + " :Wildcard in toplevel domain\n";
			break;
		case ERR_UNKNOWNCOMMAND:
			msg += " " + commandName + " :Unknown command\n";
			break;
		case ERR_NOMOTD:
			msg += " :MOTD File is missing\n";
			break;
		case ERR_NOADMININFO:
			msg += " " + commandName + " :No administrative info available\n";
			break;
		case ERR_NONICKNAMEGIVEN:
			msg += " :No nickname given\n";
			break;
		case ERR_ERRONEUSNICKNAME:
			msg += " " + commandName + " :Erroneus nickname\n";
			break;
		case ERR_NICKNAMEINUSE:
			msg += " * :Nickname is already in use.\n";
			break;
		case ERR_NICKCOLLISION:
			msg += " " + commandName + " :Nickname collision KILL\n";
			break;
		case ERR_USERNOTINCHANNEL:
			msg += " " + commandName + " :They aren't on that channel\n";
			break;
		case ERR_NOTONCHANNEL:
			msg += " " + commandName + " :You're not on that channel\n";
			break;
		case ERR_USERONCHANNEL:
			msg += " " + commandName + " :is already on channel\n";
			break;
		case ERR_NOLOGIN:
			msg += " " + commandName + " :User not logged in\n";
			break;
		case ERR_SUMMONDISABLED:
			msg += " :SUMMON has been disabled\n";
			break;
		case ERR_USERSDISABLED:
			msg += " :USERS has been disabled\n";
			break;
		case ERR_NOTREGISTERED:
			msg += " :You have not registered\n";
			break;
		case ERR_NEEDMOREPARAMS:
			msg += " " + commandName + " :Not enough parameters\n";
			break;
		case ERR_ALREADYREGISTRED:
			msg += " :You may not reregister\n";
			break;
		case ERR_NOPERMFORHOST:
			msg += " :Your host isn't among the privileged\n";
			break;
		case ERR_PASSWDMISMATCH:
			msg += " :Password incorrect\n";
			break;
		case ERR_YOUREBANNEDCREEP:
			msg += " :You are banned from this server\n";
			break;
		case ERR_KEYSET:
			msg += " " + commandName + " :Channel key already set\n";
			break;
		case ERR_CHANNELISFULL:
			msg += " " + commandName + " :Cannot join channel (+l)\n";
			break;
		case ERR_UNKNOWNMODE:
			msg += " :is unknown mode char to me\n";
			break;
		case ERR_INVITEONLYCHAN:
			msg += " " + commandName + " :Cannot join channel (+i)\n";
			break;
		case ERR_BANNEDFROMCHAN:
			msg += " " + commandName + " :Cannot join channel (+b)\n";
			break;
		case ERR_BADCHANNELKEY:
			msg += " " + commandName + " :Cannot join channel (+k)\n";
			break;
		case ERR_NOPRIVILEGES:
			msg += " :Permission Denied- You're not an IRC operator\n";
			break;
		case ERR_CHANOPRIVSNEEDED:
			msg += " " + command.getArgument(0) + " :You're not channel operator\n";
			break;
		case ERR_CANTKILLSERVER:
			msg += " :You cant kill a server!\n";
			break;
		case ERR_NOOPERHOST:
			msg += " :No O-lines for your host\n";
			break;
		case ERR_UMODEUNKNOWNFLAG:
			msg += " :Unknown MODE flag\n";
			break;
		case ERR_USERSDONTMATCH:
			msg += " :Cant change mode for other users\n";
			break;
		default:
			msg += "UNKNOWN ERROR\n";
			break;
	}
	logger.logUserMessage(msg, user, OUT);
	write(user.getFd(), msg.c_str(), msg.size());
}

