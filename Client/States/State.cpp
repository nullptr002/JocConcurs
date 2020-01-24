#include "State.h"

#include "..\Client.h"

#include <iostream>

// pointer catre singurul obiect Client
Client* State::s_client = &Client::getInstance();

// dreptunghi care acopera tot ecranul, va fi folosit pentru fade-ul intre tranzitiile game state-urilor
sf::RectangleShape State::s_transitionRectangle;

// folosit ca sa chem o singura data cateva functii pentru dreptunghiul de mai sus
bool State::isTransitionRectangleInitialized = false;

State::State()
{
	if (!isTransitionRectangleInitialized)
	{
		s_transitionRectangle.setSize(sf::Vector2f(200,200));

		std::cout << "hello from state.h" << std::endl;
		s_transitionRectangle.setFillColor(sf::Color::Red);
		isTransitionRectangleInitialized = true;
	}
}

void State::pollEvents(sf::Event& evnt, sf::RenderWindow& window)
{
	if (evnt.type == sf::Event::Closed)
	{
		window.close();
	}
}
