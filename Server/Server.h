#pragma once

#include <vector>
#include <string>
#include <memory>

#include <SFML/Network.hpp>

#include "SClient.h"

class Server
{
public:
	static Server& getInstance();

	void run();

	void acceptNewClients();

private:
	Server();
	~Server();

	Server(Server& other)			= delete;
	void operator=(Server& other)	= delete;

private:
	sf::TcpListener m_listener;
	std::vector<std::unique_ptr<SClient>> m_clients;

	int m_connectedClients = 0;
	int m_maxConnectedClients = 4;
};

