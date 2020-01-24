#include "Client.h"

#include "States/MenuState.h"
#include <iostream>

Client& Client::getInstance()
{
	static Client instance;

	return instance;
}

void Client::run()
{
	static sf::Event evnt; // pentru ca event e keyword in c++ lol

	// ceas pentru obtinerea delta time
	static sf::Clock deltaClock;

	// delta time, folosit ca indiferent de performanta pc-ului, jocul sa se miste la fel
	static sf::Time deltaTime;

	while (m_window.isOpen())
	{
		m_states.at(static_cast<int>(selectedState))->pollEvents(evnt, m_window);

		m_states.at(static_cast<int>(selectedState))->update(deltaTime);

		m_states.at(static_cast<int>(selectedState))->draw(m_window);

		deltaTime = deltaClock.restart();
		virtualCursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_window)));
	}
}

const sf::RectangleShape& Client::getVirtualCursor() const
{
	return virtualCursor;
}

Client::Client()
{
	m_states.emplace_back(std::make_unique<MenuState>());

	std::cout << m_window.getSettings().antialiasingLevel << std::endl;
}

Client::~Client()
{
}