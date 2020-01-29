#pragma once

#include <SFML/Network.hpp>
#include <string>

class SClient
{
public:
	SClient();

public:
	sf::TcpSocket socket;
	std::string name;
	std::string remoteIp;

	sf::Packet toSend;
	sf::Packet toReceive;

	sf::Clock disconnectDelayClock;

	bool inLobby = false;
};

