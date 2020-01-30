#pragma once

#include <SFML/Network.hpp>
#include <string>
#include <vector>
#include <memory>

class SClient
{
public:
	SClient();

	void sendToEveryone(std::vector<std::unique_ptr<SClient>> &clients);

public:
	sf::TcpSocket socket;
	std::string name;
	std::string remoteIp;

	sf::Packet toSend;
	sf::Packet toReceive;
	sf::Packet packetToSendToEveryone[4];

	sf::Clock disconnectDelayClock;

	int id = -1;

	bool inLobby = false;
	bool hasReceivedInitialInfo = false;
	bool hasReceivedInitialInfoFlag = false;

	bool updated[4]{ 0,0,0,0 };
};

