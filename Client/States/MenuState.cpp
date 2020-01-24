#include "MenuState.h"
#include "../Client.h"

#include <iostream>


MenuState::MenuState()
{
	hexagon.setOutlineThickness(-5);
	hexagon.setOutlineColor(sf::Color::Black);
	hexagon.setOrigin(hexagon.getLocalBounds().width / 2, hexagon.getLocalBounds().height / 2);
	hexagon.setPosition(300, 300);
}

void MenuState::pollEvents(sf::Event& evnt, sf::RenderWindow& window)
{
	while (window.pollEvent(evnt))
	{
		State::pollEvents(evnt, window);
	}
}

void MenuState::update(sf::Time& deltaTime)
{
	if (hexagon.getGlobalBounds().intersects(s_client->getVirtualCursor().getGlobalBounds()))
	{
		hexagon.setFillColor(sf::Color::Green);
	}
	else
	{
		hexagon.setFillColor(sf::Color::Red);
	}
}

void MenuState::draw(sf::RenderWindow& window)
{
	window.clear();
	window.draw(*s_transitionRectangle);
	window.draw(hexagon);
	window.display();
}

void MenuState::changeState(GameStateEnum to)
{
}
