#pragma once

#include "State.h"

#include "../Button.h"

class MenuState : public State
{
public:
	MenuState();
	
	~MenuState() {}

	void pollEvents(sf::Event &evnt, sf::RenderWindow& window)		override;
	void update(sf::Time &deltaTime)								override;
	void draw(sf::RenderWindow &window)								override;

	void sendToServer(sf::TcpSocket& socket)						override;
	void receiveFromServer(sf::TcpSocket& socket)					override;

	void changeState(GameStateEnum to)								override;

private:
	void loadButtons();

private:
	sf::Texture simple;
	sf::Texture hovered;
	sf::Texture clicked;
	Button *button = nullptr;
};