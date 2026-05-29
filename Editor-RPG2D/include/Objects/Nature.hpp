#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/GameObject.hpp"
#include "Editors/MapEditor/Map/GameObjectsOnMap.hpp"

class NaturePrefab : public GameObject {
public:
	NaturePrefab(std::wstring name, std::shared_ptr<Animations> animations, sf::Vector2i origin, std::shared_ptr<Collider> collider);
	~NaturePrefab();
};

class Nature : public GameObjectOnMap {
public:

	Nature(std::shared_ptr<GameObject> prefab);
	~Nature();
};