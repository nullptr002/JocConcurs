#pragma once

#include <array>
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
	void captureCells(int who, int i, int j, int i2 = -1, int j2 = -1);

private:
	sf::RectangleShape background;

	const std::array<const sf::Color, 5> color = {
		sf::Color(85,221,225),
		sf::Color(225,102,212),
		sf::Color(225,218,121),
		sf::Color(72,245,101),
		sf::Color(64, 48, 120)
	};

	int me = -1;
	int currentTurn = 0;
	std::array<std::string, 4>* playerNames;

	sf::Vector2f selectedPosition{ -200,-200 };
	sf::Vector2i selectedIJ;

	struct Cell
	{
		sf::RectangleShape square;
		int whose;

		bool selected;
		bool nearSelected;
		bool nearNearSelected;
	};

	std::array<std::array<Cell, 7>, 7> map;

	struct PlayerUI
	{
		sf::RectangleShape icon;
		sf::Texture iconTexture;

		sf::Text nameText;

		int cellsNumber;
		sf::Text cellsNumberText;
	};

	std::array<PlayerUI, 4> playerUI;

	bool toSendFlag = false;

	bool alive[4] = { true,true,true,true };
};