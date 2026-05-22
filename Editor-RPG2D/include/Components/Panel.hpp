#pragma once
#include "GUIManager.hpp"

class Panel : public Element {
public:
	static int _margin;
	sf::IntRect _rect;

	Panel(sf::Vector2i size, sf::Vector2i position);
	~Panel();

	sf::Vector2i getSize();
	void setPosition(sf::Vector2i position);
	sf::Vector2i getPosition();

	virtual void cursorHover();
	virtual void handleEvent(const sf::Event& event);
	virtual void update();
	virtual void draw();
};
