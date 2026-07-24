#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/Object.hpp"
#include "Cursor.hpp"

class CursorOnBuilding : public Cursors::CursorWithObject {
public:

	CursorOnBuilding();
	~CursorOnBuilding();

	virtual void update();
	virtual void handleEvent(const sf::Event& event);
	virtual void draw();
};
