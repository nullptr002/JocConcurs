#include "IntroState.h"

#include "../Client.h"
#include "../EasingFunctions.h"
#include <iostream>

sf::Texture IntroState::backgroundTexture;

IntroState::IntroState()
	: text("Se stabileste conexiunea la server...", Client::getFont(), 20)
{
	backgroundTexture.loadFromFile("resources/bg1.png");
	background.setSize(sf::Vector2f(1280, 720));
	background.setTexture(&backgroundTexture);

	text.setPosition(Client::constants().window.width / 2, Client::constants().window.height / 4 * 3);
}

void IntroState::pollEvents(sf::Event& evnt, sf::RenderWindow& window)
{
	State::pollEvents(evnt,window);
}

void IntroState::update(sf::Time& deltaTime)
{
	text.setOrigin(text.getLocalBounds().width / 2, 0.0f);

	if (startingOpening)
	{
		opening();
	}

	if (transitionClock.getElapsedTime().asSeconds() > durationIfEverythingOk && Client::getInstance().connected == true)
	{
		changeState();
	}
	else if (transitionClock.getElapsedTime().asSeconds() > durationIfEverythingOk && Client::getInstance().connected == false)
	{
		text.setString("Nu s-a putut face conexiunea cu server-ul.");
	}

	if (transitioning)
	{
		transition();

		if (transitionClock.getElapsedTime().asSeconds() > transitionDuration / 2)
		{
			loadTheOtherState(GameStateEnum::menu);
		}
	}
}

void IntroState::draw(sf::RenderWindow& window)
{
	window.clear();
	window.draw(background);
	window.draw(text);
	window.draw(*s_transitionRectangle);
	window.display();
}

void IntroState::sendToServer(sf::TcpSocket& socket)
{
}

void IntroState::receiveFromServer(sf::TcpSocket& socket)
{
}

void IntroState::loadTheOtherState(GameStateEnum which)
{
	Client::getInstance().setState(which);
}

sf::Texture* const IntroState::getBgTexture()
{
	return &backgroundTexture;
}

void IntroState::opening()
{
	static const auto func = getEasingFunction(easing_functions::EaseInQuint);

	float time = transitionClock.getElapsedTime().asSeconds();
	
	if (time < transitionDuration / 2.0)
	{
		s_transitionRectangle->setFillColor(sf::Color(0, 0, 0, 255 - time * (1 / (transitionDuration / 2)) * 255 * func(time / (transitionDuration / 2))));
	}

	if (time >= transitionDuration / 2.0f)
	{
		startingOpening = false;
	}
}
