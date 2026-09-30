#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/GameObject.hpp"
#include "Objects/PlacedGameObject.hpp"

class Building;

class WindowPrefab : public GameObject {
public:
	WindowPrefab(std::wstring name, std::weak_ptr<Animations> animations, sf::Vector2i origin, std::shared_ptr<Collider> collider, std::shared_ptr<Mesh> mesh);
	~WindowPrefab();
};

class Window : public PlacedGameObject {
public:
	std::weak_ptr<Building> _building;
	int _level = -1;

	Window(std::weak_ptr<GameObject> prefab, std::weak_ptr<Building> building, int level);
	~Window();
	void draw(sf::RenderTarget& target, float scale);
	virtual void draw();
	
};