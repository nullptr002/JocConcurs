#include "Button.h"

#include "Client.h"

#include <iostream>

Button::Button(sf::Vector2f size, sf::Texture* simple, sf::Texture* hovered, sf::Texture* clicked, std::function<void()> onClick, sf::Texture* disabled)
	:m_body(size), m_simpleTexture(simple), m_hoveredTexture(hovered), m_clickedTexture(clicked), m_onClick(onClick), m_disabledTexture(disabled)
{
	if (m_simpleTexture)
	{
		m_body.setTexture(m_simpleTexture);
	}

	m_body.setOrigin(m_body.getSize() / 2.0f);
}

void Button::pollEvents(sf::Event& evnt)
{
	// pentru ca e mai usor sa folosesc "virtualMousePtr" in loc de "Client::getInstance().getVirtualCursor()"
	const sf::RectangleShape *const virtualMousePtr = &Client::getInstance().getVirtualCursor();

	
	if (m_isDisabled)
	{
		m_body.setTexture(m_disabledTexture);
	}
	else if (!m_isDisabled)
	{
		if (virtualMousePtr->getGlobalBounds().intersects(m_body.getGlobalBounds()))
		{
			m_isHovered = true;

			if (m_hoveredTexture && !m_isClicked)
			{
				m_body.setTexture(m_hoveredTexture);
			}
		}
		else
		{
			m_isHovered = false;
			m_isClicked = false;

			if (m_simpleTexture)
			{
				m_body.setTexture(m_simpleTexture);
			}
		}

		if (evnt.type == sf::Event::MouseButtonPressed && m_isHovered == true)
		{
			if (evnt.mouseButton.button == sf::Mouse::Left)
			{
				m_isClicked = true;

				if (m_clickedTexture)
				{
					m_body.setTexture(m_clickedTexture);
				}
			}
		}
		else if (evnt.type == sf::Event::MouseButtonReleased && m_isHovered == true && m_isClicked == true)
		{
			if (evnt.mouseButton.button == sf::Mouse::Left)
			{
				m_onClick();

				m_isClicked = false;

				if (m_hoveredTexture)
				{
					m_body.setTexture(m_hoveredTexture);
				}
			}
		}
	}
}

void Button::update()
{

}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_body, states);
}

void Button::setPosition(sf::Vector2f pos)
{
	m_body.setPosition(pos);
}

void Button::disable()
{
	if (m_disabledTexture)
	{
		m_isDisabled = true;

		m_isClicked = false;
		m_isHovered = false;
	}
}

void Button::enable()
{
	m_isDisabled = false;
}
