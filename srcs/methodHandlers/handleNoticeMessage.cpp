//
// Created by Ivan on 18.06.2022.
//

#include "../../includes/Server.hpp"

void Server::handleNoticeMessage(const Command &command) {
	if(command.getArguments().size() == 0) {
		return sendError(command, ERR_NORECIPIENT);
	}
	if(command.getArguments().size() < 2) {
		return sendError(command, ERR_NOTEXTTOSEND);
	}

	std::string reciverNick = command.getArgument(0);
	if (reciverNick.at(0) == '#') {
		sendMessageToChannel(command);
	} else {
		sendMessageToUser(command, "NOTICE");
	}
}