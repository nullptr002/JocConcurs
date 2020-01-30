#include <iostream>
#include "SClient.h"

SClient::SClient()
{
	socket.setBlocking(false);
}

void SClient::sendToEveryone(std::vector<std::unique_ptr<SClient>> &clients)
{
	for (int i = 0; i < clients.size() - 1; i++)
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
						std::cout << "Clientul [" << this->id << "] a trimis clientului [" << clients[i]->id << "]" << std::endl;
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
	std::cout << std::endl;
}
