//
// Created by Ivan on 07.05.2022.
//

int 	Server::privmsg(const std::string &str, User &user)
{
	if (str.size() == 0)
		return (Error(user, ERR_NORECIPIENT,));
	if (str.size() == 1)
		return (Error(user, ERR_NOTEXTTOSEND));

	std::
	<std::string> receivers = split(msg.getParams()[0], ',', false);
	std::set<std::string> uniqReceivers;

	if (receivers.front()[0] == '#' || receivers.front()[0] == '&'))
		return (Error(user, ERR_NOSUCHNICK, msg.getParams()[0]));

	while (receivers.size() > 0)
	{
		// checking if there contains dublicate receiver
		if (uniqReceivers.find(receivers.front()) != uniqReceivers.end())
			return (Error(user, ERR_TOOMANYTARGETS, receivers.front()));
		// if receiver is channel
		if (receivers.front()[0] == '#' || receivers.front()[0] == '&')
		{
			// checking if there such a channel
			if (!this->containsChannel(receivers.front()))
				return (Error(user, ERR_NOSUCHNICK, receivers.front()));
			// check that the current user is in the channel
			if (!this->channels[receivers.front()]->containsNickname(user.getNickname()))
				return (sendError(user, ERR_CANNOTSENDTOCHAN, receivers.front()));
		}
			// checking if there such a nickname
		else if (!this->containsNickname(receivers.front()))
			return (Error(user, ERR_NOSUCHNICK, msg.getParams()[0]));
		uniqReceivers.insert(receivers.front());
		receivers.pop();
	}
	return 0;
}