#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/GameObject.hpp"
#include "Objects/PlacedGameObject.hpp"

class Building;

class DoorPrefab : public GameObject {
public:
	DoorPrefab(std::wstring name, std::weak_ptr<Animations> animations, sf::Vector2i origin, std::shared_ptr<Collider> collider, std::shared_ptr<Mesh> mesh);
	~DoorPrefab();
};

class Door : public PlacedGameObject {
public:
	std::weak_ptr<Building> _building;

	Door(std::weak_ptr<GameObject> prefab, std::weak_ptr<Building> building);
	~Door();
	void draw(sf::RenderTarget& target, float scale);
	virtual void draw();
	
};