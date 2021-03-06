#pragma once

#include <vector>
#include <string>
#include <memory>
#include <random>

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

	int m_playersInLobby = 0;

	float m_disconnectTime = 3.0f;

	int currentTurn = -1;

	std::random_device dev;
	std::mt19937 rng;
};

