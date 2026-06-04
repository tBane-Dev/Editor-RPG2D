#pragma once
#include <SFML/Graphics.hpp>
#include "RenderWindow.hpp"

namespace Main {
	class Cursor {
	public:
		sf::Vector2i _position;

		Cursor();
		~Cursor();

		void update();
		void handleEvent(const sf::Event& event);
		void draw();
	};

	extern std::shared_ptr<Cursor> cursor;
}
