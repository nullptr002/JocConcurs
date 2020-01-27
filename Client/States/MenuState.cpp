#include "MenuState.h"
#include "../Client.h"

#include <iostream>


MenuState::MenuState()
	: nameText(name, Client::getFont(), 26)
{

	nameMessageBoxTexture.loadFromFile("resources/message.png");
	nameMessageBox.setTexture(nameMessageBoxTexture);
	nameMessageBox.setOrigin(nameMessageBox.getLocalBounds().width / 2, nameMessageBox.getLocalBounds().height / 2);
	nameMessageBox.setPosition(Client::constants().window.width / 2, Client::constants().window.height / 2);

	nameText.setPosition(Client::constants().window.width / 2, Client::constants().window.height / 2 - 6);

	loadButtons();
}

void MenuState::pollEvents(sf::Event& evnt, sf::RenderWindow& window)
{

	while (window.pollEvent(evnt))
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
				ok->pollEvents(evnt);
			}
		}
		else
		{
			if (!transitioning)
			{
				joaca->pollEvents(evnt);
			}
		}
	}
}

void MenuState::update(sf::Time& deltaTime)
{
	if (s_client->getName() == "")
	{
		nameText.setString(name);
		nameText.setOrigin(nameText.getLocalBounds().width / 2, 0);
	}
	else
	{
		if (!transitioning)
		{
			joaca->update();
		}
		else
		{
			State::transition();
		}
	}
}

void MenuState::draw(sf::RenderWindow& window)
{
	window.clear(sf::Color(150,50,50));

	window.draw(*joaca);

	if (transitioning)
	{
		window.draw(*s_transitionRectangle);
	}

	if (s_client->getName() == "")
	{
		window.draw(nameMessageBox);
		window.draw(nameText);
		if (name.size() > 3)
		{
			window.draw(*ok);
		}
	}

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
			nameReadyToSend = true;
			s_client->setName(name);
		}
	}
}

void MenuState::receiveFromServer(sf::TcpSocket& socket)
{
}

void MenuState::changeState(GameStateEnum to)
{
	transitioning = true;
	transitionClock.restart();
}

void MenuState::loadButtons()
{
	if (!joaca)
	{
		joacaSimple.loadFromFile("resources/1.png");
		joacaHovered.loadFromFile("resources/2.png");
		joacaClicked.loadFromFile("resources/3.png");

		joaca = new Button(sf::Vector2f(350, 91), joacaSimple, joacaHovered, joacaClicked, [&]() { changeState(GameStateEnum::menu); });

		joaca->setPosition(sf::Vector2f(Client::constants().window.width / 2, Client::constants().window.height / 4 * 1));
	}

	if (!ok)
	{
		okSimple.loadFromFile("resources/ok1.png");
		okHovered.loadFromFile("resources/ok2.png");
		okClicked.loadFromFile("resources/ok3.png");

		ok = new Button(sf::Vector2f(171, 44), okSimple, okHovered, okClicked, [&]() { nameReadyToSend = true; toSendPacket << 1 << name; });

		ok->setPosition(nameMessageBox.getPosition() + sf::Vector2f(0.0f, 75.0f));
	}
}
