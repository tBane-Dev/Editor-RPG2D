#pragma once
#include "Editors/MapEditor/Map/GameObjectOnMap.hpp"
#include "Editors/MapEditor/Map/GameObjectsOnMap.hpp"

class SelectedGameObjectOnMap {
public: 
	std::weak_ptr<GameObjectOnMap> _object;
	sf::Vector2i _offset;

	SelectedGameObjectOnMap(std::weak_ptr<GameObjectOnMap> object, sf::Vector2i offset);
	~SelectedGameObjectOnMap();
};