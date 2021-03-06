#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <memory>

class Client;
enum class GameStateEnum;

class State 
{
public:
	State();
	
	virtual ~State() {}

	virtual void pollEvents(sf::Event &evnt, sf::RenderWindow &window);
	virtual void update(sf::Time &deltaTime)				= 0;
	virtual void draw(sf::RenderWindow &window)				= 0;

	virtual void sendToServer(sf::TcpSocket &socket)		= 0;
	virtual void receiveFromServer(sf::TcpSocket& socket)	= 0;

	virtual void loadTheOtherState(GameStateEnum which)		= 0;

protected:
	void transition();

	void changeState();

protected:
	static Client *s_client;
	static sf::RectangleShape *s_transitionRectangle;

	static bool transitioning;
	static sf::Clock transitionClock;

	static sf::Packet toSendPacket;
	static sf::Packet toReceivePacket;

	static double transitionDuration;
private:

	friend class Client;
};