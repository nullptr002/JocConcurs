#include "Button.h"

void Button::pollEvents(sf::Event& evnt)
{
}

void Button::update()
{
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(body, states);
}
