#include "Client.h"

Client& Client::getInstance()
{
	static Client instance;

	return instance;
}

Client::Client()
{
}

Client::~Client()
{
}
