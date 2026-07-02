#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/Object.hpp"

enum class CursorOnMapState { Idle, Drawing };

class CursorOnMap {
public:
	sf::Vector2i _position;
	std::weak_ptr<Object> _object = std::weak_ptr<Object>();
	int _frame = 0;
	CursorOnMapState _state;

	CursorOnMap();
	~CursorOnMap();

	void update();
	void handleEvent(const sf::Event& event);
	void draw();
};
