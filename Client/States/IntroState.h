#pragma once

#include "State.h"

class IntroState : public State
{
public:
	IntroState();

	~IntroState() {}

	void pollEvents(sf::Event& evnt, sf::RenderWindow& window)		override;
	void update(sf::Time& deltaTime)								override;
	void draw(sf::RenderWindow& window)								override;

	void sendToServer(sf::TcpSocket& socket)						override;
	void receiveFromServer(sf::TcpSocket& socket)					override;

	void loadTheOtherState(GameStateEnum which)						override;

	static sf::Texture* const getBgTexture();

private:
	void opening();

private:
	sf::RectangleShape background;
	static sf::Texture backgroundTexture;

	sf::Text text;

	bool startingOpening = true;

	float durationIfEverythingOk = 3.0f;

	friend class MenuState;
	friend class LobbyState;
};