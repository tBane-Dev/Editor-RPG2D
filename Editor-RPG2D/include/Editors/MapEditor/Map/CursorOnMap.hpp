#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/Object.hpp"

class CursorOnMap {
public:
	sf::Vector2i _position;
	std::shared_ptr<Object> _object = nullptr;
	int _frame = 0;

	CursorOnMap();
	~CursorOnMap();

	void update();
	void handleEvent(const sf::Event& event);
	void draw();
};
