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
		int getBottomWallY(int x);
		bool canPlaceWindow(sf::Vector2i windowPosition);
		bool canPlaceWallMounted(sf::Vector2i windowPosition);
	
		virtual void update();
		virtual void handleEvent(const sf::Event& event);
		virtual void draw();
	};

}