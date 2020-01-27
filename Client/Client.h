#pragma once

// libs
#include <array>
#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

class State;

// enum cu game state-uri (pentru indexarea dupa nume a elementelor din vector)
enum class GameStateEnum
{
	intro,
	menu,
	lobby,
	optiuni
};

// Clasa de baza, singleton (nu pot exista 2 instante)
// Aici se intampla tot. Un singur obiect e creat la pornirea programului, distrus la inchiderea lui.
class Client
{
// Metode publice (+constantele, nu au nevoie de getters si setters)
public:
	// modul prin care alte clase pot obtine acces la singurul obiect
	static Client& getInstance();

	
	// main loop, self-explanatory
	void run();

	
	// clasa anonima cu toate constantele
	struct {
		// constante legate de fereastra
		struct
		{
			int width = 1280;
			int height = 720;
		} window;

		// constante legate de networking
		struct
		{
			std::string server_ip = "192.168.0.200";
			unsigned int port = 20000;
		} network;

	} static const constants() {
		return decltype(constants())();
	}

	
	const sf::RectangleShape& getVirtualCursor() const;

	
	static const sf::Font& getFont();

	const std::string& getName() const;
	void setName(std::string l_name);

// Metode private
private:
	// Constructor privat, clasa nu trebuie sa poata fi instantiata din exteriorul ei
	Client();

	// Destructor privat, clasa nu trebuie sa poata fi distrusa din exterior
	~Client();

	// Copy constructor sters, clasa nu trebuie sa poata fi copiata
	Client(Client& other) = delete;

	// Copy assignment operator sters, clasa nu trebuie sa poata fi copiata
	void operator=(Client& other) = delete;


	void handleServerConnection();

// Variabile private
private:
	// asta e doar ca anti-aliasing-ul sa fie setat la 16.
	sf::ContextSettings settings{ 0,0,16,1,1,0,0 };

	// fereastra in care vor aparea graficile
	sf::RenderWindow m_window
	{ 
		sf::VideoMode(constants().window.width, constants().window.height),
		"Just a simple multiplayer turn-based strategy game", 
		sf::Style::Close | sf::Style::Titlebar,
		settings
	};

	
	// container cu toate game state-urile
	std::array<std::unique_ptr<State>, 2> m_states;
	
	// game state-ul selectat
	GameStateEnum selectedState			= GameStateEnum::intro;


	sf::TcpSocket socket;

	sf::Packet packetToSend;

	sf::Packet packetToReceive;

	bool connected = false;
	

	std::string name = "";

	// ca sa testez mai usor coliziunea mouse-ului cu butoanele
	sf::RectangleShape virtualCursor	= sf::RectangleShape(sf::Vector2f(1.0f,1.0f));


	// singurul font folosit in tot programul, public
	static sf::Font font;

};

