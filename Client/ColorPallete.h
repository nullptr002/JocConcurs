#pragma once

#include <SFML/Graphics/Color.hpp>

// Paleta de culori folosita *aproape* peste tot.
// daca vreodata vreau sa schimb o culoare, va trebui sa modific doar aici
// (exceptie fac imaginile incarcat dintr-un fisier)

struct {
	sf::Color darkViolet	= { 21,15,34 };
	sf::Color darkerViolet	= { 12,8,18 };
	sf::Color disabled		= { 47, 34, 76 };
	sf::Color outline		= { 103, 74,168 };
	sf::Color outline2		= { 225, 150, 225 };
} const colorPallete;