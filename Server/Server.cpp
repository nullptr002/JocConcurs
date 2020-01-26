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

	1 - nume
	2 - 

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
			sf::Socket::Status status = m_clients[i]->socket.receive(m_clients[i]->toReceive);

			if (status == sf::Socket::Status::Done)
			{
				int code = 0;

				m_clients[i]->toReceive >> code;

				if (code == 1)
				{
					m_clients[i]->toReceive >> m_clients[i]->name;
					std::cout << "Clientul [" << m_clients[i]->socket.getRemoteAddress().toString() << "] ";
					std::cout << "si-a setat numele \"" << m_clients[i]->name << "\".";
				}

				m_clients[i]->toReceive.clear();
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

		std::cout << "Un nou client s-a conectat la server: [";
		std::cout << m_clients[m_clients.size() - 2]->socket.getRemoteAddress().toString() << "]" << std::endl;
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
