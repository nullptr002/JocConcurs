#pragma once

#include "State.h"

#include "../Button.h"

class MenuState : public State
{
public:
	MenuState();
	
	~MenuState();

	void pollEvents(sf::Event &evnt, sf::RenderWindow& window)		override;
	void update(sf::Time &deltaTime)								override;
	void draw(sf::RenderWindow &window)								override;

	void sendToServer(sf::TcpSocket& socket)						override;
	void receiveFromServer(sf::TcpSocket& socket)					override;

	void loadTheOtherState(GameStateEnum which)						override;

private:
	void loadButtons();

private:
	sf::RectangleShape background;

	sf::RectangleShape nameMessageBox;
	sf::Text nameMessageBoxText;

	Button* continua = nullptr;
	sf::Texture continuaSimple;
	sf::Texture continuaHovered;
	sf::Texture continuaClicked;
	sf::Texture continuaDisabled;

	sf::RectangleShape nameContainer;
	std::string name = "";
	sf::Text nameText;

	bool nameReadyToSend = false;
	bool switchToLobby = false;
};