#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/GameObject.hpp"
#include "Editors/MapEditor/Map/GameObjectsOnMap.hpp"

class NaturePrefab : public GameObject {
public:
	NaturePrefab(std::wstring name, std::weak_ptr<Animations> animations, sf::Vector2i origin, std::shared_ptr<Collider> collider, std::shared_ptr<Mesh> mesh);
	~NaturePrefab();
};

class Nature : public GameObjectOnMap {
public:

	Nature(std::weak_ptr<GameObject> prefab);
	~Nature();
};