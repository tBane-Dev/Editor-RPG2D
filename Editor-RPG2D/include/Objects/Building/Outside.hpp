#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/GameObject.hpp"
#include "Editors/MapEditor/Map/GameObjectOnMap.hpp"

class Building;

class Outside : public GameObjectOnMap {
public:
	std::weak_ptr<Building> _building;
	sf::Texture _texture;

	Outside(std::weak_ptr<Building> building = std::weak_ptr<Building>());
	~Outside();

	void setTexture(sf::Texture texture);
	virtual void draw();
};