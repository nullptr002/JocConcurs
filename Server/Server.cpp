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
	3 - 

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
					// nimic nu se intampla. acest cod era necesar pentru teste la inceput
					// si pentru detectarea conexiunii daca server-ul are acelasi ip cu cel putin un client
				}
				else if (code == 2)
				{
					m_clients[i]->toReceive >> m_clients[i]->name;

					std::cout << "Clientul [" << m_clients[i]->remoteIp << "] ";
					std::cout << "si-a setat numele \"" << m_clients[i]->name << "\".";
					std::cout << std::endl;
				}
				else if (code == 3)
				{
					m_clients[i]->inLobby = true;
					
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
					std::cout << "Clientul [" << m_clients[i]->remoteIp << "] ";
					std::cout << "cu numele [\"" << m_clients[i]->name << "\"] s-a deconectat." << std::endl;
					m_clients.erase(m_clients.begin() + i);
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
