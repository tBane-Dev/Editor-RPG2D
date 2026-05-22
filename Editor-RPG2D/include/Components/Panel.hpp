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

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();
};
