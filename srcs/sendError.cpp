//
// Created by Ivan on 01.06.2022.
//
#include "../includes/allAnswers.hpp"
#include "../includes/User.hpp"
#include "../includes/Server.hpp"


void sendError(const User &user, int err, const std::string &arg1, const std::string &arg2)
{
	std::string	msg = ":Megaserver ";
	std::stringstream	ss;
	ss << err;
	msg += ss.str() + " " + user.getNick();
	switch (err)
	{
		case ERR_NOSUCHNICK:
			msg += " " + arg1 + " :No such nick/channel\n";
			break;
		case ERR_NOSUCHSERVER:
			msg += " " + arg1 + " :No such server\n";
			break;
		case ERR_NOSUCHCHANNEL:
			msg += " " + arg1 + " :No such channel\n";
			break;
		case ERR_CANNOTSENDTOCHAN:
			msg += " " + arg1 + " :Cannot send to channel\n";
			break;
		case ERR_TOOMANYCHANNELS:
			msg += " " + arg1 + " :You have joined too many channels\n";
			break;
		case ERR_WASNOSUCHNICK:
			msg += " " + arg1 + " :There was no such nickname\n";
			break;
		case ERR_TOOMANYTARGETS:
			msg += " " + arg1 + " :Duplicate recipients. No arg1 delivered\n";
			break;
		case ERR_NOORIGIN:
			msg += " :No origin specified\n";
			break;
		case ERR_NORECIPIENT:
			msg += " :No recipient given (" + arg1 + ")\n";
			break;
		case ERR_NOTEXTTOSEND:
			msg += " :No text to send\n";
			break;
		case ERR_NOTOPLEVEL:
			msg += " " + arg1 + " :No toplevel domain specified\n";
			break;
		case ERR_WILDTOPLEVEL:
			msg += " " + arg1 + " :Wildcard in toplevel domain\n";
			break;
		case ERR_UNKNOWNCOMMAND:
			msg += " " + arg1 + " :Unknown command\n";
			break;
		case ERR_NOMOTD:
			msg += " :MOTD File is missing\n";
			break;
		case ERR_NOADMININFO:
			msg += " " + arg1 + " :No administrative info available\n";
			break;
		case ERR_FILEERROR:
			msg += " :File error doing \n" + arg1 + " on " + arg2 + "\n";
			break;
		case ERR_NONICKNAMEGIVEN:
			msg += " :No nickname given\n";
			break;
		case ERR_ERRONEUSNICKNAME:
			msg += " " + arg1 + " :Erroneus nickname\n";
			break;
		case ERR_NICKNAMEINUSE:
			msg += " " + arg1 + " :Nickname is already in use\n";
			break;
		case ERR_NICKCOLLISION:
			msg += " " + arg1 + " :Nickname collision KILL\n";
			break;
		case ERR_USERNOTINCHANNEL:
			msg += " " + arg1 + " " + arg2 + " :They aren't on that channel\n";
			break;
		case ERR_NOTONCHANNEL:
			msg += " " + arg1 + " :You're not on that channel\n";
			break;
		case ERR_USERONCHANNEL:
			msg += " " + arg1 + " " + arg2 + " :is already on channel\n";
			break;
		case ERR_NOLOGIN:
			msg += " " + arg1 + " :User not logged in\n";
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
			msg += " " + arg1 + " :Not enough parameters\n";
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
			msg += " " + arg1 + " :Channel key already set\n";
			break;
		case ERR_CHANNELISFULL:
			msg += " " + arg1 + " :Cannot join channel (+l)\n";
			break;
		case ERR_UNKNOWNMODE:
			msg += " " + arg1 + " :is unknown mode char to me\n";
			break;
		case ERR_INVITEONLYCHAN:
			msg += " " + arg1 + " :Cannot join channel (+i)\n";
			break;
		case ERR_BANNEDFROMCHAN:
			msg += " " + arg1 + " :Cannot join channel (+b)\n";
			break;
		case ERR_BADCHANNELKEY:
			msg += " " + arg1 + " :Cannot join channel (+k)\n";
			break;
		case ERR_NOPRIVILEGES:
			msg += " :Permission Denied- You're not an IRC operator\n";
			break;
		case ERR_CHANOPRIVSNEEDED:
			msg += " " + arg1 + " :You're not channel operator\n";
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
	write(user.getFd(), msg.c_str(), msg.size());
}

void Server::sendError(Command const &command , int errorCode )
{
	std::string	msg = ":Megaserver ";
	std::stringstream	ss;
	User &user = command.getUser();
	std::vector<std::string> const &commands = command.getArguments();
	ss << errorCode;
	msg += ss.str() + " " + user.getNick();
	switch (errorCode)
	{
		case ERR_NOSUCHNICK:
			msg += " " + commands.at(1) + " :No such nick/channel\n";
			break;
		case ERR_NOSUCHSERVER:
			msg += " " + commands.at(1) + " :No such server\n";
			break;
		case ERR_NOSUCHCHANNEL:
			msg += " " + commands.at(1) + " :No such channel\n";
			break;
		case ERR_CANNOTSENDTOCHAN:
			msg += " " + commands.at(1) + " :Cannot send to channel\n";
			break;
		case ERR_TOOMANYCHANNELS:
			msg += " " + commands.at(1) + " :You have joined too many "
								"channels\n";
			break;
		case ERR_WASNOSUCHNICK:
			msg += " " + commands.at(1) + " :There was no such nickname\n";
			break;
		case ERR_TOOMANYTARGETS:
			msg += " " + commands.at(1) + " :Duplicate recipients. No arg1 "
						  "delivered\n";
			break;
		case ERR_NOORIGIN:
			msg += " :No origin specified\n";
			break;
		case ERR_NORECIPIENT:
			msg += " :No recipient given (" + commands.at(1) + ")\n";
			break;
		case ERR_NOTEXTTOSEND:
			msg += " :No text to send\n";
			break;
		case ERR_NOTOPLEVEL:
			msg += " " + commands.at(1) + " :No toplevel domain specified\n";
			break;
		case ERR_WILDTOPLEVEL:
			msg += " " + commands.at(1) + " :Wildcard in toplevel domain\n";
			break;
		case ERR_UNKNOWNCOMMAND:
			msg += " " + commands.at(1) + " :Unknown command\n";
			break;
		case ERR_NOMOTD:
			msg += " :MOTD File is missing\n";
			break;
		case ERR_NOADMININFO:
			msg += " " + commands.at(1) + " :No administrative info "
								 "available\n";
			break;
		case ERR_FILEERROR:
			msg += " :File error doing \n" + commands.at(1) + " on " + commands.at(2) +
					"\n";
			break;
		case ERR_NONICKNAMEGIVEN:
			msg += " :No nickname given\n";
			break;
		case ERR_ERRONEUSNICKNAME:
			msg += " " + commands.at(1) + " :Erroneus nickname\n";
			break;
		case ERR_NICKNAMEINUSE:
			msg += " " + commands.at(1) + " :Nickname is already in use\n";
			break;
		case ERR_NICKCOLLISION:
			msg += " " + commands.at(1) + " :Nickname collision KILL\n";
			break;
		case ERR_USERNOTINCHANNEL:
			msg += " " + commands.at(1) + " " + commands.at(2) + " :They aren't on that "
									  "channel\n";
			break;
		case ERR_NOTONCHANNEL:
			msg += " " + commands.at(1) + " :You're not on that channel\n";
			break;
		case ERR_USERONCHANNEL:
			msg += " " + commands.at(1) + " " + commands.at(2) + " :is already on "
											 "channel\n";
			break;
		case ERR_NOLOGIN:
			msg += " " + commands.at(1) + " :User not logged in\n";
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
			msg += " " + commands.at(1) + " :Not enough parameters\n";
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
			msg += " " + commands.at(1) + " :Channel key already set\n";
			break;
		case ERR_CHANNELISFULL:
			msg += " " + commands.at(1) + " :Cannot join channel (+l)\n";
			break;
		case ERR_UNKNOWNMODE:
			msg += " " + commands.at(1) + " :is unknown mode char to me\n";
			break;
		case ERR_INVITEONLYCHAN:
			msg += " " + commands.at(1) + " :Cannot join channel (+i)\n";
			break;
		case ERR_BANNEDFROMCHAN:
			msg += " " + commands.at(1) + " :Cannot join channel (+b)\n";
			break;
		case ERR_BADCHANNELKEY:
			msg += " " + commands.at(1) + " :Cannot join channel (+k)\n";
			break;
		case ERR_NOPRIVILEGES:
			msg += " :Permission Denied- You're not an IRC operator\n";
			break;
		case ERR_CHANOPRIVSNEEDED:
			msg += " " + commands.at(1) + " :You're not channel operator\n";
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
	write(user.getFd(), msg.c_str(), msg.size());
}
