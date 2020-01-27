#pragma once

#include "State.h"

#include "../Button.h"

class MenuState : public State
{
public:
	MenuState();
	
	~MenuState() {}

	void pollEvents(sf::Event &evnt, sf::RenderWindow& window)		override;
	void update(sf::Time &deltaTime)								override;
	void draw(sf::RenderWindow &window)								override;

	void sendToServer(sf::TcpSocket& socket)						override;
	void receiveFromServer(sf::TcpSocket& socket)					override;

	void changeState(GameStateEnum to)								override;

private:
	void loadButtons();

private:
	sf::Texture joacaSimple;
	sf::Texture joacaHovered;
	sf::Texture joacaClicked;
	Button *joaca = nullptr;

	sf::Sprite nameMessageBox;
	sf::Texture nameMessageBoxTexture;

	Button* ok = nullptr;
	sf::Texture okSimple;
	sf::Texture okHovered;
	sf::Texture okClicked;

	std::string name = "";
	sf::Text nameText;

	bool nameReadyToSend = false;
};