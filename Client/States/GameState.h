#pragma once

#include <SFML/Graphics.hpp>
#include "State.h"

class GameState : public State
{
public:
	GameState();
	~GameState() {};

	void pollEvents(sf::Event& evnt, sf::RenderWindow& window)		override;
	void update(sf::Time& deltaTime)								override;
	void draw(sf::RenderWindow& window)								override;

	void sendToServer(sf::TcpSocket& socket)						override;
	void receiveFromServer(sf::TcpSocket& socket)					override;

	void loadTheOtherState(GameStateEnum which)						override;
private:

private:
	sf::RectangleShape background;

};