#include "MenuState.h"
#include "../Client.h"
#include "IntroState.h"

#include "../ColorPallete.h"

#include <iostream>


MenuState::MenuState()
	:	background(sf::Vector2f(Client::constants().window.width, Client::constants().window.height)), nameText(name, Client::getFont(), 26),
		nameMessageBox(sf::Vector2f(404, 241)),
		nameMessageBoxText("Alege-ti un nume", Client::getFont(), 35),
		nameContainer(sf::Vector2f(150.0f, 40.0f))
{
	background.setTexture(IntroState::getBgTexture());

	nameMessageBox.setOutlineColor(colorPallete.outline2);
	nameMessageBox.setOutlineThickness(-1);

	nameMessageBox.setFillColor(colorPallete.darkViolet);
	nameMessageBox.setOrigin(static_cast<int>(nameMessageBox.getLocalBounds().width / 2), static_cast<int>(nameMessageBox.getLocalBounds().height / 2));
	nameMessageBox.setPosition(Client::constants().window.width / 2, Client::constants().window.height / 2);

	nameMessageBoxText.setPosition(nameMessageBox.getPosition() + sf::Vector2f(0.0f, -100.0f));

	nameText.setPosition(Client::constants().window.width / 2, Client::constants().window.height / 2 - 6);

	nameContainer.setFillColor(colorPallete.darkerViolet);
	nameContainer.setOutlineColor(colorPallete.outline);
	nameContainer.setOutlineThickness(-1);
	nameContainer.setOrigin(nameContainer.getSize() / 2.0f);
	nameContainer.setPosition(nameText.getPosition() + sf::Vector2f(0.0f, 18.0f));

	loadButtons();
}

MenuState::~MenuState()
{
	delete continua;
}

void MenuState::pollEvents(sf::Event& evnt, sf::RenderWindow& window)
{
	State::pollEvents(evnt, window);

	if (s_client->getName() == "")
	{
		if (evnt.type == sf::Event::TextEntered)
		{
			if (isalnum(evnt.text.unicode) && name.size() < 12)
			{
				name += evnt.text.unicode;
			}
			else if (evnt.text.unicode == '\b' && name.size() > 0)
			{
				name.pop_back();
			}
		}

		if (name.size() > 3)
		{
			continua->enable();
		}
		else
		{
			continua->disable();
		}

		continua->pollEvents(evnt);
	}
}

void MenuState::update(sf::Time& deltaTime)
{
	nameMessageBoxText.setOrigin(static_cast<int>(nameMessageBoxText.getLocalBounds().width / 2), 0.0f);

	nameText.setString(name);
	nameText.setOrigin(nameText.getLocalBounds().width / 2, 0);

	if (transitioning && switchToLobby == false)
	{
		State::transition();
	}

	if (nameText.getLocalBounds().width > nameContainer.getSize().x)
	{
		nameContainer.setSize(sf::Vector2f(nameText.getLocalBounds().width + 7.0f, nameContainer.getSize().y));
		nameContainer.setOrigin(nameContainer.getSize() / 2.0f);
	}
	else if (nameText.getLocalBounds().width + 10.0f < nameContainer.getSize().x)
	{
		nameContainer.setSize(sf::Vector2f(150.0f, 40.0f));
		nameContainer.setOrigin(nameContainer.getSize() / 2.0f);
	}

	if (transitioning && switchToLobby == true)
	{
		State::transition();

		if (transitionClock.getElapsedTime().asSeconds() > transitionDuration / 2)
		{
			loadTheOtherState(GameStateEnum::lobby);
		}
	}
}

void MenuState::draw(sf::RenderWindow& window)
{
	window.clear(sf::Color(255, 0, 0));

	window.draw(background);

	if (s_client->getName() == "")
	{
		window.draw(nameMessageBox);
		window.draw(nameMessageBoxText);

		window.draw(nameContainer);

		window.draw(nameText);
		window.draw(*continua);
	}

	window.draw(*s_transitionRectangle);

	window.display();
}

void MenuState::sendToServer(sf::TcpSocket& socket)
{
	if (nameReadyToSend)
	{
		sf::Socket::Status status = socket.send(toSendPacket);

		if (status == sf::Socket::Status::Done)
		{
			toSendPacket.clear();
			switchToLobby = true;
			nameReadyToSend = false;
			s_client->setName(name);

			changeState();
		}
	}
}

void MenuState::receiveFromServer(sf::TcpSocket& socket)
{
}

void MenuState::loadTheOtherState(GameStateEnum which)
{
	Client::getInstance().setState(which);
}

void MenuState::loadButtons()
{
	if (!continua)
	{
		continuaSimple.loadFromFile("resources/continua2.png");
		continuaHovered.loadFromFile("resources/continua3.png");
		continuaClicked.loadFromFile("resources/continua4.png");
		continuaDisabled.loadFromFile("resources/continua1.png");

		continua = new Button
		(
			sf::Vector2f(171, 44),
			&continuaSimple,
			&continuaHovered,
			&continuaClicked,
			[&]() { nameReadyToSend = true; toSendPacket << 2 << name; },
			&continuaDisabled
		);

		continua->setPosition(nameMessageBox.getPosition() + sf::Vector2f(0.0f, 75.0f));
		continua->disable();
	}
}
