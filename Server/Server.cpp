#include "Server.h"

#include <iostream>

Server& Server::getInstance()
{
	static Server instance;
	
	return instance;
}

void Server::run()
{
	while (true)
	{
		if (m_listener.accept(*m_clients.back()) == sf::Socket::Status::Done)
		{
			m_clients.emplace_back(std::make_unique<sf::TcpSocket>());
			m_clients.back()->setBlocking(false);
			m_connectedClients++;

			std::cout << "New client connected: " << m_clients[m_clients.size() - 2]->getRemoteAddress() << std::endl;
		}

		for (size_t i = 0; i < m_clients.size() - 1; i++)
		{
			sf::Socket::Status status = m_clients[i]->receive(packet);

			//std::cout << "Status: " << status << std::endl;
			if (status == sf::Socket::Status::Done)
			{
				std::cout << "aici" << std::endl;

				packet >> packetContent;
				std::cout << packetContent << std::endl;

				packet.clear();
				packetContent.empty();
			}
		}
	}
}

Server::Server()
{
	m_clients.emplace_back(std::make_unique<sf::TcpSocket>());
	m_clients[0]->setBlocking(false);

	m_listener.listen(20000);
	m_listener.setBlocking(false);

	std::cout << m_clients.size() << std::endl;
}

Server::~Server()
{
}
