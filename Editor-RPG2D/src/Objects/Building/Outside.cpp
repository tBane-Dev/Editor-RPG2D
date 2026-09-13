#pragma once
#include "Objects/Building/Outside.hpp"
#include "RenderWindow.hpp"
#include "DebugLog.hpp"
#include "Objects/Building/Building.hpp"
#include "Editors/MapEditor/Editor.hpp"
#include "Editors/BuildingsEditor/Editor.hpp"
#include "Editors/MapEditor/Map/CursorOnMap.hpp"
#include "Editors/MapEditor/Map/PlacedGameObjects.hpp"

Outside::Outside(std::weak_ptr<Building> building) : PlacedGameObject(std::weak_ptr<GameObject>()){
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

		

		if (Main::editor_manager->get_back() == BuildingsEditor::editor) {


			std::shared_ptr<Roof> roof = buildingPrefab->_roof;
			sf::Vector2i roofOverhangSize = roof ? roof->_roofOverhangSize : sf::Vector2i(0, 0);
			float scale = BuildingsEditor::editor->_building_panel->_building->_scale;

			sf::Vector2f buildingPosition(_building.lock()->getPosition());
			float topOffset = roof ? roof->getTopOffset(1.f) : 0.f;

			sf::Vector2f position(
				buildingPosition.x - (float)(roofOverhangSize.x) * scale,
				buildingPosition.y - (float)(topOffset + roofOverhangSize.y) * scale
			);


			sf::Sprite sprite(_texture);
			sprite.setPosition(position);
			sprite.setScale(sf::Vector2f(scale, scale));
			Main::render_window->draw(sprite);
		}

		if (Main::editor_manager->get_back() == MapEditor::editor) {
			std::shared_ptr<Roof> roof = buildingPrefab->_roof;
			sf::Vector2i roofOverhangSize = roof ? roof->_roofOverhangSize : sf::Vector2i(0, 0);
			sf::Sprite sprite(_texture);
			sf::Vector2f position = sf::Vector2f(_position.x - roofOverhangSize.x, _position.y - roofOverhangSize.y - (int)(_texture.getSize().y));
			sprite.setPosition(position);
			Main::render_window->draw(sprite);
		}

		
	}
}