#pragma once

#include <memory>
#include <vector>
#include <string>

#include <SFML/Network.hpp>

class Server
{
public:
	static Server& getInstance();

	void run();

private:
	Server();
	~Server();

	Server(Server& other)			= delete;
	void operator=(Server& other)	= delete;

private:

	sf::TcpListener m_listener;
	std::vector<std::unique_ptr<sf::TcpSocket>> m_clients;

	sf::Packet packet;
	std::string packetContent;

	int m_connectedClients = 0;
	int m_maxConnectedClients = 4;
};

