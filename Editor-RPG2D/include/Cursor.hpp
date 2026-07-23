#pragma once
#include <SFML/Graphics.hpp>
#include "RenderWindow.hpp"
#include "Objects/Object.hpp"

namespace Cursors {
	class Cursor {
	public:
		sf::Vector2i _position;

		Cursor();
		~Cursor();

		virtual void update();
		virtual void handleEvent(const sf::Event& event);
		virtual void draw();
	};

	extern std::shared_ptr<Cursor> cursor;


	extern std::vector<std::vector<std::vector<bool>>> circle_brushes;
	extern std::vector<std::vector<std::vector<bool>>> square_brushes;

	enum class CursorWithObjectState { Idle, Drawing };

	class CursorWithObject : public Cursor {
	public:

		sf::Vector2i _globalPosition;
		std::weak_ptr<Object> _object = std::weak_ptr<Object>();
		CursorWithObjectState _state;

		CursorWithObject();
		~CursorWithObject();

		virtual void update();
		virtual void handleEvent(const sf::Event& event);
		virtual void draw();
	};

	
}
