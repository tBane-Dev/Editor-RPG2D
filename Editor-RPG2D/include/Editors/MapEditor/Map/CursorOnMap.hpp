#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/Object.hpp"
#include "Cursor.hpp"
#include "Objects/SelectedPlacedGameObject.hpp"

class CursorOnMap : public Cursors::CursorWithObject {
public:
    std::vector<std::shared_ptr<SelectedPlacedGameObject>> _prevSelectedObjects;
    std::vector<std::shared_ptr<SelectedPlacedGameObject>> _selectedObjects;
	bool _isDragging = false;
	bool _isSelecting = false;
	sf::IntRect _selectionRect;

	CursorOnMap();
	~CursorOnMap();

	void removeFromSelected(std::shared_ptr<GameObject> object);

	virtual void update();
	virtual void handleEvent(const sf::Event& event);
	virtual void draw();
};
