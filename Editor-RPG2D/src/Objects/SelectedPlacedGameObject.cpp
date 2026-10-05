#include "Objects/SelectedPlacedGameObject.hpp"

SelectedPlacedGameObject::SelectedPlacedGameObject(std::shared_ptr<PlacedGameObject> object, sf::Vector2i offset) {
	_object = object;
	_offset = offset;
}

SelectedPlacedGameObject::~SelectedPlacedGameObject() {

}