#pragma once
#include <SFML/Graphics.hpp>

class CursorOnMap {
public:
	sf::Vector2i _position;

	CursorOnMap();
	~CursorOnMap();

	void update();
	void handleEvent(const sf::Event& event);
	void draw();
};

extern std::shared_ptr<CursorOnMap> cursorOnMap;