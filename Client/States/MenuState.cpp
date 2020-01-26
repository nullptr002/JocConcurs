#include "MenuState.h"
#include "../Client.h"

#include <iostream>


MenuState::MenuState()
{
	loadButtons();
}

void MenuState::pollEvents(sf::Event& evnt, sf::RenderWindow& window)
{
	State::pollEvents(evnt, window);

	while (window.pollEvent(evnt))
	{
		if (!transitioning)
		{
			button->pollEvents(evnt);
		}
	}
}

void MenuState::update(sf::Time& deltaTime)
{
	if (!transitioning)
	{
		button->update();
	}
	else
	{
		State::transition();
	}
}

void MenuState::draw(sf::RenderWindow& window)
{
	window.clear(sf::Color(150,50,50));

	window.draw(*button);

	if (transitioning)
	{
		window.draw(*s_transitionRectangle);
	}

	window.display();
}

void MenuState::sendToServer(sf::TcpSocket& socket)
{
}

void MenuState::receiveFromServer(sf::TcpSocket& socket)
{
}

void MenuState::changeState(GameStateEnum to)
{
	transitioning = true;
	transitionClock.restart();
}

void MenuState::loadButtons()
{
	if (!button)
	{
		simple.loadFromFile("resources/1.png");
		hovered.loadFromFile("resources/2.png");
		clicked.loadFromFile("resources/3.png");

		button = new Button(sf::Vector2f(350, 91), simple, hovered, clicked, [&]() { changeState(GameStateEnum::menu); });

		button->setPosition(sf::Vector2f(Client::constants().window.width / 2, Client::constants().window.height / 4 * 1));
	}
}
