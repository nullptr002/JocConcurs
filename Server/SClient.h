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

	sf::Packet toSend;
	sf::Packet toReceive;
};

