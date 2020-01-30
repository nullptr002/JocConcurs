#pragma once

#include "State.h"
#include "../Button.h"
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

	sf::Texture suntGataSimple;
	sf::Texture suntGataHovered;
	sf::Texture suntGataClicked;
	sf::Texture suntGataDisabled;

	sf::Text playerCount;

	struct Player
	{
		bool connected						= false;
		bool ready							= false;
		std::string name					= "";

		sf::RectangleShape nameContainer	= sf::RectangleShape(sf::Vector2f(226, 50));
		sf::Text nameText;

		sf::RectangleShape container		= sf::RectangleShape(sf::Vector2f(226, 320));
		sf::Texture texture;

		Button readyButton
		{
			sf::Vector2f(226, 50),
			nullptr,
			nullptr,
			nullptr,
			[&]() { },
			nullptr
		};
	};

	std::array<Player, 4> players;

	int me = -1;

	bool readyFlag = false;

private:
};