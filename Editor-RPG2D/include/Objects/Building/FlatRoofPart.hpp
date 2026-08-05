#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/GameObject.hpp"
#include "Editors/MapEditor/Map/GameObjectOnMap.hpp"

class FlatRoofPartPrefab : public GameObject {
public:
	int _id = -1;

	FlatRoofPartPrefab(std::wstring name, std::weak_ptr<Animations> animations, sf::Vector2i origin, std::shared_ptr<Mesh> mesh, int id);
	~FlatRoofPartPrefab();
};

class FlatRoofPart : public GameObjectOnMap {
public:

	sf::IntRect _textureRect;

	FlatRoofPart(std::weak_ptr<GameObject> prefab, sf::IntRect textureRect = sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(32, 32)));
	~FlatRoofPart();

	virtual void draw(sf::RenderTarget& target, float scale = 1.f);
};