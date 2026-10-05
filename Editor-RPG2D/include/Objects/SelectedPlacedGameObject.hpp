#pragma once
#include "Objects/PlacedGameObject.hpp"

class SelectedPlacedGameObject {
public: 
	std::shared_ptr<PlacedGameObject> _object;
	sf::Vector2i _offset;

	SelectedPlacedGameObject(std::shared_ptr<PlacedGameObject> object, sf::Vector2i offset);
	~SelectedPlacedGameObject();
};