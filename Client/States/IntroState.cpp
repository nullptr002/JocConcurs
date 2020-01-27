#include "IntroState.h"

#include "../Client.h"

IntroState::IntroState()
	: text("Se stabileste conexiunea cu server-ul...", Client::getFont(), 30)
{
	backgroundTexture.loadFromFile("resources/bg1.png");
	background.setSize(sf::Vector2f(1280, 720));
	background.setTexture(&backgroundTexture);

	text.setPosition(Client::constants().window.width / 2, Client::constants().window.height / 4 * 3);
}

void IntroState::pollEvents(sf::Event& evnt, sf::RenderWindow& window)
{
	while (window.pollEvent(evnt))
	{
		State::pollEvents(evnt,window);

	}
}

void IntroState::update(sf::Time& deltaTime)
{
	text.setOrigin(text.getLocalBounds().width / 2, 0);
}

void IntroState::draw(sf::RenderWindow& window)
{
	window.clear();
	window.draw(background);
	window.draw(text);
	window.display();
}

void IntroState::sendToServer(sf::TcpSocket& socket)
{
}

void IntroState::receiveFromServer(sf::TcpSocket& socket)
{
}

void IntroState::changeState(GameStateEnum to)
{
}
