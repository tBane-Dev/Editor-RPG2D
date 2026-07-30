#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/Object.hpp"
#include "Cursor.hpp"
#include "Editors/MapEditor/Map/SelectedGameObjectOnMap.hpp"

class CursorOnMap : public Cursors::CursorWithObject {
public:
    std::vector<std::shared_ptr<SelectedGameObjectOnMap>> _prevSelectedObjects;
    std::vector<std::shared_ptr<SelectedGameObjectOnMap>> _selectedObjects;
	sf::IntRect _selectionRect;

	CursorOnMap();
	~CursorOnMap();

	virtual void update();
	virtual void handleEvent(const sf::Event& event);
	virtual void draw();
};
