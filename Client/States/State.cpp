#include "State.h"

#include "..\Client.h"

#include <iostream>

// pointer catre singurul obiect Client
Client* State::s_client = &Client::getInstance();

// dreptunghi care acopera tot ecranul, va fi folosit pentru fade-ul intre tranzitiile game state-urilor
// daca va intrebati de ce este variabila globala si nu membra, 
// am dat de un bug inexplicabil si pare sa functioneze daca nu apartine clasei State
sf::RectangleShape transitionRectangle = []() -> sf::RectangleShape {

	sf::RectangleShape shape;
	shape.setSize(sf::Vector2f(Client::getInstance().constants.window.width, Client::getInstance().constants.window.height));
	shape.setFillColor(sf::Color::Red);

	return shape;
} ();

// pointer la obiectul global de mai sus
sf::RectangleShape* State::s_transitionRectangle = nullptr;

State::State()
{
	if(!s_transitionRectangle)
		s_transitionRectangle = &transitionRectangle;
}

void State::pollEvents(sf::Event& evnt, sf::RenderWindow& window)
{
	if (evnt.type == sf::Event::Closed)
	{
		window.close();
	}
}
