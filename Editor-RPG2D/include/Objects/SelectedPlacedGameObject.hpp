#pragma once
#include "Objects/PlacedGameObject.hpp"

class SelectedPlacedGameObject {
public: 
	std::weak_ptr<PlacedGameObject> _object;
	sf::Vector2i _offset;

	SelectedPlacedGameObject(std::weak_ptr<PlacedGameObject> object, sf::Vector2i offset);
	~SelectedPlacedGameObject();
};