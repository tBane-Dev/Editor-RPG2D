#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/Object.hpp"
#include "Cursor.hpp"

class CursorOnMap : public Cursors::CursorWithObject {
public:

	CursorOnMap();
	~CursorOnMap();

	virtual void update();
	virtual void handleEvent(const sf::Event& event);
	virtual void draw();
};
