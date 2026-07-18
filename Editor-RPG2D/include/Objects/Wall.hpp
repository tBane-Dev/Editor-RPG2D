#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/GameObject.hpp"
#include "Editors/MapEditor/Map/GameObjectsOnMap.hpp"

class WallPrefab : public GameObject {
public:
	int _id = -1;

	WallPrefab(std::wstring name, std::weak_ptr<Animations> animations, sf::Vector2i origin, std::shared_ptr<Collider> collider, std::shared_ptr<Mesh> mesh, int id);
	~WallPrefab();
};

class Wall : public GameObjectOnMap {
public:

	sf::IntRect _textureBottomRect;
	sf::IntRect _textureTopRect;

	Wall(std::weak_ptr<GameObject> prefab, sf::IntRect textureBottomRect = sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(32, 32)), sf::IntRect textureTopRect = sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(32, 32)));
	~Wall();

	virtual void draw(float scale = 1.f);
};