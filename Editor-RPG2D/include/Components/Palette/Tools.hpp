#pragma once
#include <SFML/Graphics.hpp>

class Tools {
public:
	int _main_margin;
	int _top_margin;
	int _outer_margin;
	int _inner_margin;
	sf::IntRect _rect;

	std::unique_ptr<sf::Text> _text;

	Tools();
	~Tools();
	
	sf::Vector2i getPosition();
	sf::Vector2i getSize();
	virtual void setPosition(sf::Vector2i position);

	virtual void cursorHover();
	virtual void handleEvent(const sf::Event& event);
	virtual void update();
	virtual void draw();
};