#pragma once

// libs
#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

class State;

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

	// clasa anonima cu toate constantele, publice si encapsulate
	struct {
		// constante legate de fereastra
		struct
		{
			int width = 1280;
			int height = 720;
		} window;

		// constante legate de network
		struct
		{
			std::string server_ip = "";
		} network;

	} const constants;

	const sf::RectangleShape& getVirtualCursor() const;

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

// Variabile private
private:
	// asta e doar ca anti-aliasing-ul sa fie setat la 16.
	sf::ContextSettings settings{ 0,0,16,1,1,0,0 };

	// fereastra in care vor aparea graficile
	sf::RenderWindow m_window
	{ 
		sf::VideoMode(constants.window.width, constants.window.height),
		"Just a simple multiplayer turn-based strategy game", 
		sf::Style::Close | sf::Style::Titlebar,
		settings
	};

	// container cu toate game state-urile
	std::vector<std::unique_ptr<State>> m_states;

	// enum cu game state-uri (pentru indexarea dupa nume a elementelor din vector)
	enum class GameStateEnum
	{
		menu
	};
	
	// game state-ul selectat
	GameStateEnum selectedState			= GameStateEnum::menu;
	
	// ca sa testez mai usor coliziunea mouse-lui cu butoanele
	sf::RectangleShape virtualCursor	= sf::RectangleShape(sf::Vector2f(1.0f,1.0f));

};

