#include "LobbyState.h"

#include "../Client.h"
#include "IntroState.h"

#include <iostream>

LobbyState::LobbyState()
	: background(sf::Vector2f(Client::constants().window.width, Client::constants().window.height))
{
	background.setTexture(IntroState::getBgTexture());

	players[0].texture.loadFromFile("");
	players[1].texture.loadFromFile("");
	players[2].texture.loadFromFile("");
	players[3].texture.loadFromFile("");

	
}

void LobbyState::pollEvents(sf::Event& evnt, sf::RenderWindow& window)
{
}

void LobbyState::update(sf::Time& deltaTime)
{
	if (transitioning)
	{
		State::transition();
	}
}

void LobbyState::draw(sf::RenderWindow& window)
{
	window.clear(sf::Color(150, 50, 50));

	window.draw(background);
	window.draw(*s_transitionRectangle);

	window.display();
}

void LobbyState::sendToServer(sf::TcpSocket& socket)
{

}

void LobbyState::receiveFromServer(sf::TcpSocket& socket)
{
	sf::Socket::Status status = socket.receive(toReceivePacket);

	if (status == sf::Socket::Status::Done)
	{

	}
}

void LobbyState::loadTheOtherState(GameStateEnum which)
{
}
