#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

// clasa buton, folosita pentru toate butoanele
class Button : public sf::Drawable
{
public:
	// Constructor (e lung, dar nu am ce sa fac lmao)
	Button(sf::Vector2f size, sf::Texture* simple, sf::Texture* hovered, sf::Texture* clicked, std::function<void()> onClick, sf::Texture* disabled = nullptr);

	// ca sa pot detecta daca mouse-ul se afla pe buton sau daca butonul a fost apasat
	void pollEvents(sf::Event &evnt);
	
	// ca sa schimb textura si sa chem m_onClick() daca e nevoie
	void update();
	
	// override la functia virtuala draw a clasei sf::Drawable, de la care e mostenita
	// trebuie override la asta daca vreau sa chem window.draw(numeButon)
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setPosition(sf::Vector2f pos);

	void setScale(sf::Vector2f factor);

	sf::FloatRect getGlobalBounds() const;

	void setDisabledTexture(sf::Texture* texture);
	void setSimpleTexture(sf::Texture* texture);
	void setHoveredTexture(sf::Texture* texture);
	void setClickedTexture(sf::Texture* texture);

	void setOnClickLambda(std::function<void()> lambda);

	void disable();
	void enable();

	void lockClicked();

private:


private:
	// dreptunghi pe care vor fi aplicate texturile
	sf::RectangleShape m_body;

	// self-explanatory
	bool m_isHovered = false;
	bool m_isClicked = false;

	bool m_isDisabled = false;

	bool m_lockedClicked = false;

	// texturi pentru cand nu se intampla nimic, cand mouse-ul e pe buton sau cand a fost apasat
	sf::Texture* m_simpleTexture;
	sf::Texture* m_hoveredTexture;
	sf::Texture* m_clickedTexture;

	sf::Texture* m_disabledTexture;

	std::function<void()> m_onClick;
};