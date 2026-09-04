#pragma once
#include "Objects/Building/Outside.hpp"
#include "RenderWindow.hpp"
#include "DebugLog.hpp"
#include "Objects/Building/Building.hpp"
#include "Editors/MapEditor/Editor.hpp"
#include "Editors/MapEditor/Map/CursorOnMap.hpp"
#include "Editors/MapEditor/Map/GameObjectsOnMap.hpp"

Outside::Outside(std::weak_ptr<Building> building) : GameObjectOnMap(std::weak_ptr<GameObject>()){
	_type = ObjectType::Outside;
	_building = building;
}

Outside::~Outside() {

}

void Outside::setTexture(sf::Texture texture) {
	_texture = texture;
}

void Outside::draw() {
	
	std::shared_ptr<BuildingPrefab> buildingPrefab = std::dynamic_pointer_cast<BuildingPrefab>(_building.lock()->_prefab.lock());

	if (_building.lock()->_renderOutsideLook) {

		std::shared_ptr<Roof> roof = buildingPrefab->_roof;
		sf::Vector2i roofOverhangSize = roof ? roof->_roofOverhangSize : sf::Vector2i(0, 0);
		sf::Sprite sprite(_texture);
		sprite.setPosition(sf::Vector2f(_position.x - roofOverhangSize.x, _position.y - roofOverhangSize.y - (int)_texture.getSize().y));
		Main::render_window->draw(sprite);
	}
}