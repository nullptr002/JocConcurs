#pragma once

#include "State.h"
#include <array>

class LobbyState : public State
{
public:
	LobbyState();

	~LobbyState() {}

	void pollEvents(sf::Event& evnt, sf::RenderWindow& window)		override;
	void update(sf::Time& deltaTime)								override;
	void draw(sf::RenderWindow& window)								override;

	void sendToServer(sf::TcpSocket& socket)						override;
	void receiveFromServer(sf::TcpSocket& socket)					override;

	void loadTheOtherState(GameStateEnum which)						override;

private:
	sf::RectangleShape background;

	struct Player
	{
		bool connected					= false;
		std::string name				= "";

		sf::RectangleShape container	= sf::RectangleShape(sf::Vector2f(226, 320));
		sf::Texture texture;
	};

	std::array<Player, 4> players;

	int me = -1;

private:
};