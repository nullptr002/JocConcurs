#include "State.h"

#include "../EasingFunctions.h"
#include "../Client.h"

#include <iostream>

// pointer catre singurul obiect Client
Client* State::s_client = &Client::getInstance();

bool State::transitioning = false;
sf::Clock State::transitionClock;
double State::transitionDuration = 1.0;

// dreptunghi care acopera tot ecranul, va fi folosit pentru fade-ul intre tranzitiile game state-urilor
// daca va intrebati de ce este variabila globala si nu membra, 
// am dat de un bug inexplicabil si pare sa functioneze daca nu apartine clasei State
sf::RectangleShape transitionRectangle = []() -> sf::RectangleShape {

	sf::RectangleShape shape;
	shape.setSize(sf::Vector2f(Client::constants().window.width, Client::constants().window.height));
	shape.setFillColor(sf::Color::Black);

	return shape;
} ();

// pointer la obiectul global de mai sus
sf::RectangleShape* State::s_transitionRectangle = nullptr;

sf::Packet State::toSendPacket;
sf::Packet State::toReceivePacket;

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

void State::changeState()
{
	transitioning = true;
	transitionClock.restart();
}

void State::transition()
{
	static const auto func = getEasingFunction(easing_functions::EaseInQuint);

	float time = transitionClock.getElapsedTime().asSeconds();

	if (time < transitionDuration / 2.0)
	{
		s_transitionRectangle->setFillColor(sf::Color(0, 0, 0, time * (1 / (transitionDuration / 2)) * 255 * func(time / (transitionDuration / 2))));
	}
	else if (time >= transitionDuration / 2.0 && time <= transitionDuration)
	{
		s_transitionRectangle->setFillColor(sf::Color(0, 0, 0, 255 - ((time - transitionDuration / 2) * (1 / (transitionDuration / 2)) * 255 * func((time - transitionDuration / 2) / (transitionDuration / 2)))));
	}
	else if (time > transitionDuration)
	{
		s_transitionRectangle->setFillColor(sf::Color(0, 0, 0, 0));
		transitioning = false;
	}
}
