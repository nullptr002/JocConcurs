#include "LobbyState.h"

#include "IntroState.h"
#include "../Client.h"
#include "../ColorPallete.h"

#include <iostream>

LobbyState::LobbyState()
	: background(sf::Vector2f(Client::constants().window.width, Client::constants().window.height))
{
	background.setTexture(IntroState::getBgTexture());

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
		players[i].container.setOutlineThickness(-1);
		players[i].container.setPosition
		(
			Client::constants().window.width / 5 * i + Client::constants().window.width / 5,
			Client::constants().window.height / 2 - 30
		);

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

	for (const auto& player : players)
	{
		window.draw(player.container);
		window.draw(player.readyButton);
	}

	window.draw(*s_transitionRectangle);

	window.display();
}

void LobbyState::sendToServer(sf::TcpSocket& socket)
{

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

			players[me].readyButton.enable();
			players[me].container.setTexture(&players[me].texture);
			players[me].container.setFillColor(sf::Color::White);
			players[me].container.setOutlineColor(sf::Color::White);
			players[me].container.setOutlineThickness(-3);

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
		}

		toReceivePacket.clear();
	}
}

void LobbyState::loadTheOtherState(GameStateEnum which)
{
}
