#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/Object.hpp"
#include "Cursor.hpp"
#include "Objects/SelectedPlacedGameObject.hpp"
#include <map>

namespace MapEditor {
	class CursorOnMap : public Cursors::CursorWithObject {
	public:
		std::vector<std::shared_ptr<SelectedPlacedGameObject>> _prevSelectedObjects;
		std::vector<std::shared_ptr<SelectedPlacedGameObject>> _selectedObjects;
		sf::Vector2i _prevPosition = sf::Vector2i(-1, -1);
		bool _isDragging = false;
		bool _isSelecting = false;
		sf::IntRect _selectionRect;

		struct TileValues { int oldValue; int newValue; };
		struct Vector2iCompare {
			bool operator()(const sf::Vector2i& a, const sf::Vector2i& b) const {
				return a.x < b.x || (a.x == b.x && a.y < b.y);
			}
		};
		std::map<sf::Vector2i, MapEditor::CursorOnMap::TileValues, Vector2iCompare> _editedTiles;

		CursorOnMap();
		~CursorOnMap();

		void addToSelected(std::shared_ptr<PlacedGameObject> object, sf::Vector2i offset);
		void removeFromSelected(std::shared_ptr<GameObject> object);

		int nextBuildingId();

		virtual void update();
		virtual void handleEvent(const sf::Event& event);
		virtual void draw();
	};

}