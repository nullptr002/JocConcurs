#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class Client;
enum class GameStateEnum;


class State 
{
public:
	State();
	
	virtual ~State() {}

	virtual void pollEvents(sf::Event &evnt, sf::RenderWindow &window);
	virtual void update(sf::Time &deltaTime)		= 0;
	virtual void draw(sf::RenderWindow &window)		= 0;

	virtual void changeState(GameStateEnum to)		= 0;

protected:
	

protected:
	static Client *s_client;
	static sf::RectangleShape *s_transitionRectangle;

private:
};