#include "LobbyState.h"

#include "IntroState.h"
#include "../Client.h"
#include "../ColorPallete.h"

#include <iostream>

LobbyState::LobbyState()
	: background(sf::Vector2f(Client::constants().window.width, Client::constants().window.height)),
	  playerCount("1/4", Client::getFont(), 20)
{
	background.setTexture(IntroState::getBgTexture());

	playerCount.setPosition(Client::constants().window.width / 2, 50);

	players[0].texture.loadFromFile("resources/container0.png");
	players[1].texture.loadFromFile("resources/container1.png");
	players[2].texture.loadFromFile("resources/container2.png");
	players[3].texture.loadFromFile("resources/container3.png");

	suntGataDisabled.loadFromFile("resources/suntgata0.png");
	suntGataSimple.loadFromFile("resources/suntgata1.png");
	suntGataHovered.loadFromFile("resources/suntgata2.png");
	suntGataClicked.loadFromFile("resources/suntgata3.png");

	for (int i = 0; i < players.size(); i++)
	{
		players[i].container.setOrigin(players[i].container.getSize() / 2.0f);
		players[i].container.setFillColor(sf::Color::Transparent);
		players[i].container.setOutlineColor(colorPallete.outline);
		players[i].container.setOutlineThickness(1);
		players[i].container.setPosition
		(
			Client::constants().window.width / 5 * i + Client::constants().window.width / 5,
			Client::constants().window.height / 2 - 30
		);

		players[i].nameContainer.setOrigin(players[i].nameContainer.getSize() / 2.0f);
		players[i].nameContainer.setPosition
		(
			sf::Vector2f
			(
				players[i].container.getPosition().x,

				players[i].container.getPosition().y +
				players[i].container.getSize().y / 2 -
				players[i].nameContainer.getSize().y / 2
			)
		);
		players[i].nameContainer.setFillColor(sf::Color(15, 15, 15, 150));

		players[i].nameText.setCharacterSize(20);
		players[i].nameText.setPosition(players[i].nameContainer.getPosition() + sf::Vector2f(0, -10));
		players[i].nameText.setFont(Client::getFont());
		players[i].nameText.setString("");

		players[i].readyButton.setPosition
		(
			players[i].container.getPosition() +
			sf::Vector2f(0.0f, players[i].container.getSize().y / 2 + 35)
		);

		players[i].readyButton.setDisabledTexture(&suntGataDisabled);
		players[i].readyButton.setSimpleTexture(&suntGataSimple);
		players[i].readyButton.setHoveredTexture(&suntGataHovered);
		players[i].readyButton.setClickedTexture(&suntGataClicked);
		players[i].readyButton.disable();
	}
}

void LobbyState::pollEvents(sf::Event& evnt, sf::RenderWindow& window)
{
	if (me != -1)
	{
		players[me].readyButton.pollEvents(evnt);
	}
}

void LobbyState::update(sf::Time& deltaTime)
{
	{
		int l_connected = 0;
		for (int i = 0; i < players.size(); i++)
		{
			l_connected += players[i].connected;
			
			if (players[i].ready)
			{
				players[i].readyButton.lockClicked();
			}
		}

		playerCount.setString(std::to_string(l_connected) + "/4");
		playerCount.setOrigin(playerCount.getLocalBounds().width / 2, 0.0f);

		if (l_connected == 4)
		{
			playerCount.setFillColor(sf::Color(50, 205, 50));
			players[me].readyButton.enable();
		}
	}

	if (transitioning)
	{
		State::transition();
	}
	else
	{
		if (me != -1)
		{
			players[me].readyButton.update();
		}
	}
}

void LobbyState::draw(sf::RenderWindow& window)
{
	window.clear(sf::Color(150, 50, 50));

	window.draw(background);
	window.draw(playerCount);

	for (const auto& player : players)
	{
		window.draw(player.container);

		if (player.connected)
		{
			window.draw(player.nameContainer);
		}

		window.draw(player.nameText);
		window.draw(player.readyButton);
	}

	window.draw(*s_transitionRectangle);

	window.display();
}

void LobbyState::sendToServer(sf::TcpSocket& socket)
{
	if (readyFlag && players[me].ready == false)
	{	
		sf::Socket::Status status = socket.send(toSendPacket);

		if (status == sf::Socket::Status::Done)
		{
			readyFlag = false;
			players[me].ready = true;
		}
	}
}

void LobbyState::receiveFromServer(sf::TcpSocket& socket)
{
	sf::Socket::Status status = socket.receive(toReceivePacket);


	if (status == sf::Socket::Status::Done)
	{
		int code = 0;
		toReceivePacket >> code;
		std::cout << "code: " << code << std::endl;

		if (code == 3)
		{
			int playersCount = 0;
			toReceivePacket >> me >> playersCount;
			std::cout << "me: " << me << std::endl;
			std::cout << "playersCount: " << playersCount << std::endl;


			players[me].container.setTexture(&players[me].texture);
			players[me].container.setFillColor(sf::Color::White);
			players[me].container.setOutlineColor(sf::Color::White);
			players[me].container.setOutlineThickness(3);

			players[me].connected = true;

			players[me].name = s_client->getName();
			players[me].nameText.setString(players[me].name);
			players[me].nameText.setOrigin(players[me].nameText.getLocalBounds().width / 2.0f, 0.0f);

			players[me].readyButton.setOnClickLambda
			(
				[&]() 
				{
					if (!readyFlag)
					{
						readyFlag = true;

						toSendPacket.clear();
						toSendPacket << 5;
					}
				}
			);

			if (playersCount > 1)
			{
				for (int i = 0; i < playersCount; i++)
				{
					if (i != me)
					{
						int index = -1;
						toReceivePacket >> index;
						toReceivePacket >> players[index].name;
						std::cout << "player[" << index << "] is named \"" << players[index].name << "\"" << std::endl << std::endl;

						players[index].container.setTexture(&players[index].texture);
						players[index].container.setFillColor(sf::Color::White);
						players[index].connected = true;

						players[index].nameText.setString(players[index].name);
						players[index].nameText.setOrigin(players[index].nameText.getLocalBounds().width / 2.0f, 0.0f);
					}
				}
			}
		}
		else if (code == 4)
		{
			int index = -1;
			toReceivePacket >> index;
			toReceivePacket >> players[index].name;

			std::cout << "player[" << index << "] is named \"" << players[index].name << "\"" << std::endl << std::endl;

			players[index].container.setTexture(&players[index].texture);
			players[index].container.setFillColor(sf::Color::White);
			players[index].connected = true;

			players[index].nameText.setString(players[index].name);
			players[index].nameText.setOrigin(players[index].nameText.getLocalBounds().width / 2.0f, 0.0f);
		}
		else if (code == 5)
		{
			int theId = -1;
			toReceivePacket >> theId;

			players[theId].ready = true;
		}

		toReceivePacket.clear();
	}
}

void LobbyState::loadTheOtherState(GameStateEnum which)
{
}
