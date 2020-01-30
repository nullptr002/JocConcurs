#include <iostream>
#include "SClient.h"

SClient::SClient()
{
	socket.setBlocking(false);
}

void SClient::sendToEveryone(std::vector<std::unique_ptr<SClient>> &clients)
{
	for (int i = 0; i < clients.size(); i++)
	{
		if (updated[i] == 0)
		{
			if (clients[i]->id != id)
			{
				if (clients[i]->inLobby)
				{
					sf::Socket::Status status = clients[i]->socket.send(packetToSendToEveryone[i]);
					if (status == sf::Socket::Status::Done)
					{
						updated[i] = true;
						packetToSendToEveryone[i].clear();
					}
				}
				else
				{
					updated[i] = true;
					packetToSendToEveryone[i].clear();
				}

			}
			else
			{
				updated[i] = true;
				packetToSendToEveryone[i].clear();
			}
		}
	}

}
