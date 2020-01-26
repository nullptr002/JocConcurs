#include "Client.h"

#include "States/MenuState.h"
#include <iostream>

sf::Font Client::font;


Client& Client::getInstance()
{
	static Client instance;

	return instance;
}

void Client::run()
{
	sf::Event evnt; // pentru ca event e keyword in c++ lol

	// ceas pentru obtinerea delta time
	sf::Clock deltaClock;

	// delta time, folosit ca indiferent de performanta pc-ului, jocul sa se miste la fel
	sf::Time deltaTime;

	sf::Clock lol;
	while (m_window.isOpen())
	{
		if (lol.getElapsedTime().asSeconds() > 0.1f)
			m_states.at(static_cast<int>(selectedState))->pollEvents(evnt, m_window);

		m_states.at(static_cast<int>(selectedState))->update(deltaTime);

		m_states.at(static_cast<int>(selectedState))->draw(m_window);

		handleServerConnection();

		deltaTime = deltaClock.restart();
		virtualCursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_window)));
	}
}

const sf::RectangleShape& Client::getVirtualCursor() const
{
	return virtualCursor;
}

const sf::Font& Client::getFont()
{
	return font;
}

Client::Client()
	: m_states{ { std::make_unique<MenuState>() } }
{
	font.loadFromFile("resources/Franklin Gothic Demi Cond Regular.ttf");

	socket.setBlocking(false);
	int a = 1;
	packetToSend << a << name;
}

Client::~Client()
{
}

void Client::handleServerConnection()
{
	if (socket.getRemoteAddress() == sf::IpAddress::None)
	{
		connected = false;
	}
	else
	{
		connected = true;
	}

	if (!connected)
	{
		sf::Socket::Status status = socket.connect(constants().network.server_ip, constants().network.port);
		
		if (status == sf::Socket::Status::Done)
		{
			connected = true;
			std::cout << "Te-ai conectat la server!" << std::endl;
		}
	}
	else
	{
		static bool flag = false;

		if (!flag)
		{
			sf::Socket::Status status = socket.send(packetToSend);

			if (status == sf::Socket::Status::Done)
			{
				flag = true;
				packetToSend.clear();
			}
		}
		

		m_states.at(static_cast<int>(selectedState))->receiveFromServer(socket);
		m_states.at(static_cast<int>(selectedState))->sendToServer(socket);
	}
}
