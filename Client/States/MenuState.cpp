#include "MenuState.h"

#include <iostream>

MenuState::MenuState()
{

}

void MenuState::pollEvents(sf::Event& evnt, sf::RenderWindow& window)
{
	while (window.pollEvent(evnt))
	{
		State::pollEvents(evnt, window);
	}
}

void MenuState::update(sf::Time& deltaTime)
{
}

void MenuState::draw(sf::RenderWindow& window)
{
	window.clear();
	window.draw(s_transitionRectangle);
	window.display();
}

void MenuState::changeState(GameStateEnum to)
{
}
