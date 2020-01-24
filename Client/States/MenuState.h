#pragma once

#include "State.h"

class MenuState : public State
{
public:
	MenuState();
	
	~MenuState() {}

	void pollEvents(sf::Event &evnt, sf::RenderWindow& window)		override;
	void update(sf::Time &deltaTime)								override;
	void draw(sf::RenderWindow &window)								override;

	void changeState(GameStateEnum to)								override;

protected:
	

protected:

};