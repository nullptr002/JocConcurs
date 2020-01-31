#include "GameState.h"
#include "IntroState.h"
#include "../Client.h"

GameState::GameState()
	: background(sf::Vector2f(Client::constants().window.width, Client::constants().window.height))
{
	background.setTexture(IntroState::getBgTexture());
}

void GameState::pollEvents(sf::Event& evnt, sf::RenderWindow& window)
{
}

void GameState::update(sf::Time& deltaTime)
{
	if (transitioning)
	{
		State::transition();
	}
}

void GameState::draw(sf::RenderWindow& window)
{
	window.clear(sf::Color(50, 50, 200));

	window.draw(background);
	window.draw(*s_transitionRectangle);

	window.display();
}

void GameState::sendToServer(sf::TcpSocket& socket)
{
}

void GameState::receiveFromServer(sf::TcpSocket& socket)
{
}

void GameState::loadTheOtherState(GameStateEnum which)
{
}
