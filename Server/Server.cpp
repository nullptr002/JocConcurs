#include "Server.h"

Server& Server::getInstance()
{
	static Server instance;
	
	return instance;
}

Server::Server()
{
}

Server::~Server()
{
}
