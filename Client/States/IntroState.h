#pragma once

#include "State.h"

class IntroState : public State
{
public:
	IntroState();

	~IntroState() {}

	void pollEvents(sf::Event& evnt, sf::RenderWindow& window)		override;
	void update(sf::Time& deltaTime)								override;
	void draw(sf::RenderWindow& window)								override;

	void sendToServer(sf::TcpSocket& socket)						override;
	void receiveFromServer(sf::TcpSocket& socket)					override;

	void changeState(GameStateEnum to)								override;

private:


private:
	sf::RectangleShape background;
	sf::Texture backgroundTexture;

	sf::Text text;

};