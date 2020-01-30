#include "Server.h"

#include <iostream>

Server& Server::getInstance()
{
	static Server instance;
	
	return instance;
}

/*
	Coduri pachet

	Fiecare pachet va avea ca prima informatie un numar (int), si in functie de numarul ala
	decid ce fac cu restul informatiei

	0 - error?

	1 - testare conexiune
	2 - nume

	3 - lobby initial state
	4 - update lobby

*/


void Server::run()
{
	while (true)
	{
		if (m_connectedClients < m_maxConnectedClients)
		{
			acceptNewClients();
		}

		for (size_t i = 0; i < m_clients.size() - 1; i++)
		{
			//// Receiving
			sf::Socket::Status status = m_clients[i]->socket.receive(m_clients[i]->toReceive);

			if (status == sf::Socket::Status::Done)
			{
				int code = 0;

				m_clients[i]->toReceive >> code;

				if (code == 1)
				{
					// nimic nu se intampla. acest cod era necesar pentru teste la inceput
					// si pentru detectarea conexiunii daca server-ul are acelasi ip cu cel putin un client
				}
				else if (code == 2)
				{
					m_clients[i]->toReceive >> m_clients[i]->name;

					std::cout << "Clientul [" << m_clients[i]->remoteIp << "] ";
					std::cout << "si-a setat numele \"" << m_clients[i]->name << "\".";
					std::cout << std::endl;

					m_clients[i]->id = m_playersInLobby;
					m_clients[i]->inLobby = true;
					m_playersInLobby++;
				}
				else if (code == 3)
				{
					
				}

				m_clients[i]->toReceive.clear();

				m_clients[i]->disconnectDelayClock.restart();
			}
			else
			{
				// daca nu s-a primit un pachet cu code-ul 1 pentru testarea conexiunii,
				// conexiunea a picat
				if (m_clients[i]->disconnectDelayClock.getElapsedTime().asSeconds() > m_disconnectTime)
				{
					if (m_clients[i]->inLobby)
					{
						m_playersInLobby--;
					}

					std::cout << "Clientul [" << m_clients[i]->remoteIp << "] ";
					std::cout << "cu numele [\"" << m_clients[i]->name << "\"] s-a deconectat." << std::endl;
					m_clients.erase(m_clients.begin() + i);
				}
			}

			//// Sending
			if (m_clients[i]->inLobby)
			{
				if (m_clients[i]->hasReceivedInitialInfo == false)
				{
					if (m_clients[i]->hasReceivedInitialInfoFlag == false)
					{
						m_clients[i]->toSend << 3 << m_clients[i]->id << m_playersInLobby;

						for (int k = 0; k < m_clients.size(); k++)
						{
							if (m_clients[k]->inLobby == true && k != i)
							{
								m_clients[i]->toSend << k << m_clients[k]->name;
							}
						}

						m_clients[i]->hasReceivedInitialInfoFlag = true;
					}

					sf::Socket::Status status = m_clients[i]->socket.send(m_clients[i]->toSend);

					if (status == sf::Socket::Status::Done)
					{
						m_clients[i]->hasReceivedInitialInfo = true;
						m_clients[i]->toSend.clear();

						for (int j = 0; j < 4; j++)
						{
							m_clients[i]->packetToSendToEveryone[j].clear();
							m_clients[i]->packetToSendToEveryone[j] << 4;
							m_clients[i]->packetToSendToEveryone[j] << m_clients[i]->id;
							m_clients[i]->packetToSendToEveryone[j] << m_clients[i]->name;
						}
					}
				}
				else
				{
					if (std::find(std::begin(m_clients[i]->updated), std::end(m_clients[i]->updated), 0) != std::end(m_clients[i]->updated))
					{
						m_clients[i]->sendToEveryone(m_clients);
					}
				}
			}

		}
	}
}

void Server::acceptNewClients()
{
	if (m_listener.accept(m_clients.back()->socket) == sf::Socket::Status::Done)
	{
		m_clients.emplace_back(std::make_unique<SClient>());
		m_connectedClients++;

		m_clients[m_clients.size() - 2]->remoteIp = m_clients[m_clients.size() - 2]->socket.getRemoteAddress().toString();

		std::cout << "Un nou client s-a conectat la server: [";
		std::cout << m_clients[m_clients.size() - 2]->remoteIp << "]" << std::endl;

		m_clients[m_clients.size() - 2]->disconnectDelayClock.restart();
	}
}

Server::Server()
{
	m_clients.emplace_back(std::make_unique<SClient>());

	m_listener.listen(20000);
	m_listener.setBlocking(false);

	std::cout << m_clients.size() << std::endl;
}

Server::~Server()
{
}
