#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/GameObject.hpp"
#include "Objects/PlacedGameObject.hpp"

class NaturePrefab : public GameObject {
public:
	NaturePrefab(std::wstring name, std::weak_ptr<Animations> animations, sf::Vector2i origin, std::shared_ptr<Collider> collider, std::shared_ptr<Mesh> mesh);
	~NaturePrefab();
};

class Nature : public PlacedGameObject {
public:

	Nature(std::weak_ptr<GameObject> prefab);
	~Nature();
};