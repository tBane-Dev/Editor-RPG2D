#pragma once
#include "Objects/Building/Outside.hpp"
#include "RenderWindow.hpp"
#include "DebugLog.hpp"
#include "Objects/Building/Building.hpp"
#include "Editors/MapEditor/Editor.hpp"
#include "Editors/MapEditor/Map/CursorOnMap.hpp"

Outside::Outside(std::shared_ptr<Building> building) : GameObjectOnMap(std::weak_ptr<GameObject>()){
	_type = ObjectType::Outside;
	_building = building;
}

Outside::~Outside() {

}

void Outside::setTexture(sf::Texture texture) {
	_texture = texture;
}

void Outside::draw() {
	
	std::shared_ptr<BuildingPrefab> buildingPrefab = std::dynamic_pointer_cast<BuildingPrefab>(_building->_prefab.lock());

	if (!buildingPrefab->getMesh()->isPointInside(MapEditor::editor->_cursor_on_map->_globalPosition, _building->_position)) {
		sf::Sprite sprite(_texture);
		sprite.setPosition(sf::Vector2f(_position.x, _position.y - _texture.getSize().y));
		Main::render_window->draw(sprite);
	}
}