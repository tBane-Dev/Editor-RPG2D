#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/Object.hpp"
#include "Cursor.hpp"

namespace BuildingsEditor {
	class CursorOnBuilding : public Cursors::CursorWithObject {
	public:

		CursorOnBuilding();
		~CursorOnBuilding();

		bool canPlaceDoor(sf::Vector2i wallPosition);

		virtual void update();
		virtual void handleEvent(const sf::Event& event);
		virtual void draw();
	};

}