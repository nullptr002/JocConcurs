#include "GameState.h"
#include "IntroState.h"
#include "LobbyState.h"
#include "../Client.h"
#include "../ColorPallete.h"

#include <iostream>

double distanceBetween(sf::Vector2f point1, sf::Vector2f point2)
{
	const double xDiff = point1.x - point2.x;
	const double yDiff = point1.y - point2.y;

	return std::sqrt(xDiff * xDiff + yDiff * yDiff);
}

GameState::GameState()
	: background(sf::Vector2f(Client::constants().window.width, Client::constants().window.height))
{
	background.setTexture(IntroState::getBgTexture());

	sf::Vector2f startingPos = { 490.0f,210.0f };
	for (int i = 0; i < map.size(); i++)
	{
		map[i].fill({ sf::RectangleShape(sf::Vector2f(50, 50)), 4, false, false, false });
		for (int j = 0; j < map.size(); j++)
		{
			map[i][j].square.setOutlineColor(colorPallete.outline);
			map[i][j].square.setOutlineThickness(-1);
			map[i][j].square.setFillColor(sf::Color(64, 48, 120));
			map[i][j].square.setOrigin(25, 25);
			map[i][j].square.setPosition(startingPos);

			startingPos += {50.0f, 0.0f};
		}

		startingPos += {map.size() * -50.0f, 50.0f};
	}	

	map[0][0].square.setFillColor(color[0]);
	map[0][6].square.setFillColor(color[1]);
	map[6][0].square.setFillColor(color[2]);
	map[6][6].square.setFillColor(color[3]);

	map[0][0].whose = 0;
	map[0][6].whose = 1;
	map[6][0].whose = 2;
	map[6][6].whose = 3;

	playerUI[0].iconTexture.loadFromFile("resources/icon0.png");
	playerUI[1].iconTexture.loadFromFile("resources/icon1.png");
	playerUI[2].iconTexture.loadFromFile("resources/icon2.png");
	playerUI[3].iconTexture.loadFromFile("resources/icon3.png");

	for (int i = 0; i < 4; i++)
	{
		playerUI[i].icon.setSize(sf::Vector2f(170.0f, 170.0f));
		playerUI[i].icon.setTexture(&playerUI[i].iconTexture);

		playerUI[i].nameText.setCharacterSize(20);
		playerUI[i].nameText.setFont(Client::getFont());
		playerUI[i].nameText.setFillColor(sf::Color::White);

		playerUI[i].cellsNumberText.setCharacterSize(50);
		playerUI[i].cellsNumberText.setFont(Client::getFont());
		playerUI[i].cellsNumberText.setFillColor(sf::Color::White);
	}


	playerUI[0].icon.setPosition(sf::Vector2f(40.0f, 40.0f));
	playerUI[1].icon.setPosition(sf::Vector2f(1070.0f, 40.0f));
	playerUI[2].icon.setPosition(sf::Vector2f(40.0f, 510.0f));
	playerUI[3].icon.setPosition(sf::Vector2f(1070.0f, 510.0f));
}

void GameState::pollEvents(sf::Event& evnt, sf::RenderWindow& window)
{
	if (currentTurn == me)
	{
		if (evnt.type == sf::Event::MouseButtonPressed)
		{
			if (evnt.mouseButton.button == sf::Mouse::Button::Left)
			{
				bool anySelected = false;

				for (int i = 0; i < map.size(); i++)
				{
					for (int j = 0; j < map.size(); j++)
					{
						if (s_client->getVirtualCursor().getGlobalBounds().intersects(map[i][j].square.getGlobalBounds())
							&& map[i][j].whose == me)
						{
							map[i][j].selected = true;
							anySelected = true;

							selectedPosition = map[i][j].square.getPosition();
							selectedIJ = sf::Vector2i(i, j);

							for (int k = 0; k < map.size(); k++)
							{
								for (int l = 0; l < map.size(); l++)
								{
									double distance = distanceBetween(selectedPosition, map[k][l].square.getPosition());
									if (distance < 85.0)
									{
										map[k][l].nearSelected = true;
									}
									else if (distance > 85 && distance < 145)
									{
										map[k][l].square.setFillColor(color[map[k][l].whose] - sf::Color(0, 0, 0, 90));
										map[k][l].nearNearSelected = true;
									}
									else
									{
										map[k][l].nearSelected = false;
										map[k][l].nearNearSelected = false;
									}
								}
							}
						}
						else if (s_client->getVirtualCursor().getGlobalBounds().intersects(map[i][j].square.getGlobalBounds())
							&& map[i][j].nearSelected == true)
						{
							captureCells(me, i, j);
							toSendFlag = true;
							toSendPacket.clear();
							toSendPacket << 6 << 0 << me << i << j;


							do
							{
								if (currentTurn < 4)
								{
									currentTurn++;

								}
								else
								{
									currentTurn = 0;
								}
							} while (!alive[currentTurn]);
							
						}
						else if (s_client->getVirtualCursor().getGlobalBounds().intersects(map[i][j].square.getGlobalBounds())
							&& map[i][j].nearNearSelected == true)
						{
							captureCells(me, i, j, selectedIJ.x, selectedIJ.y);
							toSendFlag = true;
							toSendPacket.clear();
							toSendPacket << 6 << 1 << me << i << j << selectedIJ.x << selectedIJ.y;

							do
							{
								if (currentTurn < 4)
								{
									currentTurn++;

								}
								else
								{
									currentTurn = 0;
								}
							} while (!alive[currentTurn]);
						}
						else
						{
							map[i][j].selected = false;

							selectedPosition = sf::Vector2f(-200, -200);
						}
					}
				}

				if (!anySelected)
				{
					for (int k = 0; k < map.size(); k++)
					{
						for (int l = 0; l < map.size(); l++)
						{
							map[k][l].nearNearSelected = false;
							map[k][l].nearSelected = false;
						}
					}
				}
			}
		}
	}
}

void GameState::update(sf::Time& deltaTime)
{
	if (transitioning)
	{
		State::transition();

		static bool once = false;
		if (!once)
		{
			me = LobbyState::getMe();
			playerNames = LobbyState::getPlayerNames();
			currentTurn = LobbyState::getPlayerThatStarts();

			std::cout << currentTurn << playerNames->at(currentTurn) << " va face prima mutare." << std::endl;

			once = true;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		playerUI[i].cellsNumber = 0;
	}

	//	if (i == currentTurn)
	//	{
	//		playerUI[currentTurn].icon.setOutlineThickness(3);
	//	}
	//	else
	//	{
	//		playerUI[currentTurn].icon.setOutlineThickness(0);
	//	}
	//}

	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map.size(); j++)
		{
			if (map[i][j].selected)
			{
				map[i][j].square.setFillColor(color[map[i][j].whose] - sf::Color(0, 0, 0, 150));
				map[i][j].square.setOutlineThickness(-3);
				map[i][j].square.setOutlineColor(sf::Color::White);
			}
			else if (map[i][j].nearSelected)
			{
				map[i][j].square.setFillColor(color[map[i][j].whose] - sf::Color(0, 0, 0, 150));
				map[i][j].square.setOutlineThickness(-1);
				map[i][j].square.setOutlineColor(colorPallete.outline);
			}
			else if (map[i][j].nearNearSelected)
			{
				map[i][j].square.setFillColor(color[map[i][j].whose] - sf::Color(0, 0, 0, 90));
				map[i][j].square.setOutlineThickness(-1);
				map[i][j].square.setOutlineColor(colorPallete.outline);
			}
			else
			{
				map[i][j].square.setFillColor(color[map[i][j].whose]);
				map[i][j].square.setOutlineThickness(-1);
				map[i][j].square.setOutlineColor(colorPallete.outline);
			}

			if (map[i][j].whose < 4)
			{
				playerUI[map[i][j].whose].cellsNumber++;;
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (playerUI[i].cellsNumber == 0)
		{
			alive[i] = false;
		}
	}
}

void GameState::draw(sf::RenderWindow& window)
{
	window.clear(sf::Color(50, 50, 200));

	window.draw(background);
	window.draw(*s_transitionRectangle);
	for (const auto &row : map)
	{
		for (const auto &cell : row)
		{
			window.draw(cell.square);
		}
	}
	for (const auto& x : playerUI)
	{
		window.draw(x.icon);
	}

	window.display();
}

void GameState::sendToServer(sf::TcpSocket& socket)
{
	if (toSendFlag)
	{
		sf::Socket::Status status = socket.send(toSendPacket);

		if (status == sf::Socket::Status::Done)
		{
			toSendFlag = false;
		}
	}
}

void GameState::receiveFromServer(sf::TcpSocket& socket)
{
	sf::Socket::Status status = socket.receive(toReceivePacket);

	if (status == sf::Socket::Status::Done)
	{
		int code = -1;

		toReceivePacket >> code;

		if (code == 6)
		{
			int whatKindOfCapture;
			toReceivePacket >> whatKindOfCapture;

			int theId, theI, theJ;
			int theI2 = -1, theJ2 = -1;
			toReceivePacket >> theId >> theI >> theJ;

			if (whatKindOfCapture == 1)
			{
				toReceivePacket >> theI2 >> theJ2;
			}

			captureCells(theId, theI, theJ, theI2, theJ2);

			do
			{
				if (currentTurn < 4)
				{
					currentTurn++;

				}
				else
				{
					currentTurn = 0;
				}
			} while (!alive[currentTurn]);

			std::cout << "Current turn: " << currentTurn << std::endl;
		}

		toReceivePacket.clear();
	}
}

void GameState::loadTheOtherState(GameStateEnum which)
{
}

void GameState::captureCells(int who, int i, int j, int i2, int j2)
{
	map[i][j].whose = who;

	for (int k = 0; k < map.size(); k++)
	{
		for (int l = 0; l < map.size(); l++)
		{
			double distance = distanceBetween(map[i][j].square.getPosition(), map[k][l].square.getPosition());
			if (distance < 85 && map[k][l].whose != who && map[k][l].whose != 4)
			{
				map[k][l].whose = who;
			}
		}
	}

	if (i2 != -1)
	{
		map[i2][j2].whose = 4;
	}
}
