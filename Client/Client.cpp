#include "Client.h"

#include "States/MenuState.h"
#include "States/IntroState.h"
#include "States/LobbyState.h"

#include "ColorPallete.h"

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
		{
			while (m_window.pollEvent(evnt))
			{
				m_states.at(static_cast<int>(selectedState))->pollEvents(evnt, m_window);
				handleWindowDrag1(evnt);
			}
		}

		m_states.at(static_cast<int>(selectedState))->update(deltaTime);

		handleWindowDrag2();

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

const std::string& Client::getName() const
{
	return name;
}

void Client::setName(std::string l_name)
{
	name = l_name;
}

void Client::setState(GameStateEnum which)
{
	selectedState = which;
}

Client::Client()
	: m_states{ { std::make_unique<IntroState>(), std::make_unique<MenuState>(), std::make_unique<LobbyState>() } }
{
	font.loadFromFile("resources/ebrima.ttf");

	socket.setBlocking(false);

	packetToSend << 2 << name;
	std::cout << name << std::endl;
}

Client::~Client()
{
}

void Client::handleWindowDrag1(sf::Event& evnt)
{
	if (State::s_transitionRectangle->getOutlineThickness() != -1.0f)
	{
		State::s_transitionRectangle->setOutlineThickness(-1.0f);
	}

	if (evnt.type == sf::Event::MouseButtonPressed)
	{
		if (evnt.mouseButton.button == sf::Mouse::Left)
		{
			grabOffset = m_window.getPosition() - sf::Mouse::getPosition();
		}
	}
}

void Client::handleWindowDrag2()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && m_window.hasFocus())
	{
		State::s_transitionRectangle->setOutlineColor(colorPallete.outline2);
		isWindowGrabbed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	}
	else
	{
		State::s_transitionRectangle->setOutlineColor(sf::Color(255, 255, 255, 0));
		isWindowGrabbed = false;
	}

	if (isWindowGrabbed)
	{
		m_window.setPosition(sf::Mouse::getPosition() + grabOffset);

		State::s_transitionRectangle->setOutlineColor(sf::Color(255, 255, 255, 255));
	}
}

void Client::handleServerConnection()
{
	static bool supperficiallyConnected = false;

	sf::IpAddress addr = socket.getRemoteAddress();
	if (addr == sf::IpAddress::None)
	{
		supperficiallyConnected = false;
	}
	else
	{
		supperficiallyConnected = true;
	}

	if (!supperficiallyConnected)
	{
		sf::Socket::Status status = socket.connect(constants().network.server_ip, constants().network.port);
		
		if (status == sf::Socket::Status::Done)
		{
			supperficiallyConnected = true;
		}
	}
	else
	{
		m_states.at(static_cast<int>(selectedState))->receiveFromServer(socket);
		m_states.at(static_cast<int>(selectedState))->sendToServer(socket);

		testConnection();
	}
}

void Client::testConnection()
{
	static sf::Packet testPacket;
	static sf::Clock testClock;
	static float testCycleDuration = 1.0f;

	if (testClock.getElapsedTime().asSeconds() > testCycleDuration)
	{
		static bool wasInserted = false;

		if (!wasInserted)
		{
			testPacket << 1;
			wasInserted = true;
		}

		sf::Socket::Status status = socket.send(testPacket);

		if (status == sf::Socket::Status::Done)
		{
			testClock.restart();
			wasInserted = false;
			
			if (!connected)
			{
				connected = true;
				std::cout << "Te-ai conectat la server!" << std::endl;
			}
		}
		else if (testClock.getElapsedTime().asSeconds() > 3.0f)
		{
			connected = false;
			std::cout << "Disconnected!" << std::endl;
		}
	}
}
