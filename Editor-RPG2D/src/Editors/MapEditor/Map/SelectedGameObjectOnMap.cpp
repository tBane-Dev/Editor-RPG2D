#include "Editors/MapEditor/Map/SelectedGameObjectOnMap.hpp"

SelectedGameObjectOnMap::SelectedGameObjectOnMap(std::weak_ptr<GameObjectOnMap> object, sf::Vector2i offset) {
	_object = object;
	_offset = offset;
}

SelectedGameObjectOnMap::~SelectedGameObjectOnMap() {

}