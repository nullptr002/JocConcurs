#include <iostream>
#include "SClient.h"

SClient::SClient()
{
	socket.setBlocking(false);
}

void SClient::sendToEveryone(std::vector<std::unique_ptr<SClient>> &clients)
{
	sf::Packet packetToSendToEveryone[4];
	for (auto packet : packetToSendToEveryone)
	{
		packet << 4;
		packet << this->id;
		packet << this->name;
	}

	bool sent[4] = { 0,0,0,0 };

	while (std::find(std::begin(sent), std::end(sent), 0) != std::end(sent))
	{
		for (int i = 0; i < clients.size(); i++)
		{
			if (sent[i] == 0)
			{
				if (clients[i]->id != id)
				{
					if (clients[i]->inLobby)
					{
						sf::Socket::Status status = clients[i]->socket.send(packetToSendToEveryone[i]);
						if (status == sf::Socket::Status::Done)
						{
							sent[i] = true;
						}
					}
					else
					{
						sent[i] = true;
					}

				}
				else
				{
					sent[i] = true;
				}
			}
		}
	}

}
