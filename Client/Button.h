#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

class Button : public sf::Drawable
{
public:
	Button();

	void pollEvents(sf::Event &evnt);
	void update();
	
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:


private:
	sf::RectangleShape body;
	sf::Text text;
	
	sf::Color baseColor;
	sf::Color hoverColor;
	sf::Color clickedColor;

	std::function<void()> onClick;
};